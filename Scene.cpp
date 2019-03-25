

#define GLM_ENABLE_EXPERIMENTAL

#include "Scene.h"
#include <cassert>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>

#include "IndexObjLoader.h"

#include "Image.h"
#include "Mesh.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

Scene::Scene(const std::string &filename) {
    GenerateScene(filename);
    auto dimensions = camera.GetDimensions();
    const float maxHeight = dimensions.first;
    const float maxWidth = dimensions.second;

    std::cout << "Number of objects: " << objects.size() << std::endl;

    const int h = static_cast<int>(maxHeight); // 300;
    const int w = static_cast<int>(
        maxWidth); // static_cast<int>(camera.GetAspectRatio() * h);

    const int pixelCount = (2 * h + 1) * (2 * w + 1);
    std::cout << "Number of pixels: " << pixelCount << std::endl;
    auto *pixels = new unsigned char[3 * pixelCount];

    for (int y = -h; y <= h; ++y) {
        for (int x = -w; x <= w; ++x) {
            //            int wx = static_cast<int>(glm::round(maxWidth / w *
            //            x)); int wy = static_cast<int>(glm::round(maxHeight /
            //            h * y));
            int pixelIndex = 3 * ((h - y) * (2 * w + 1) + x + w);
            const Ray ray = camera.CreateRay(x, y);
            int minObject = -1;
            float minDistance = std::numeric_limits<float>::max();
            for (int i = 0; i < objects.size(); ++i) {
                float distance = objects[i]->GetIntersection(ray);
                if (0 < distance && distance < minDistance) {
                    minObject = i;
                    minDistance = distance;
                }
            }
            if (minObject != -1) {
                glm::vec3 normal = glm::normalize(
                    objects[minObject]->GetNormal(ray, minDistance));
                glm::vec3 objectDiffuseColor =
                    objects[minObject]->GetDiffuseColor();
                glm::vec3 objectSpecularColor =
                    objects[minObject]->GetSpecularColor();
                float objectShininess = objects[minObject]->GetShininess();
                glm::vec3 color(0);
                for (auto light : lights) {
                    const Ray shadowRay =
                        light.CreateRay(ray.GetPoint(minDistance - 0.01f));
                    bool unblocked = true;
                    for (auto object : objects) {
                        float distance = object->GetIntersection(shadowRay);
                        if (0 < distance) {
                            unblocked = false;
                            break;
                        }
                    }

                    if (unblocked) {
                        glm::vec3 viewDirection = -ray.GetDirection();
                        glm::vec3 lightDirection = shadowRay.GetDirection();
                        glm::vec3 reflectionDirection = glm::normalize(
                            2 * glm::dot(lightDirection, normal) * normal -
                            lightDirection);

                        glm::vec3 diffuseColor =
                            light.GetDiffuseColor() * objectDiffuseColor *
                            glm::clamp(glm::dot(lightDirection, normal), 0.f,
                                       1.f);
                        glm::vec3 specularColor =
                            light.GetSpecularColor() * objectSpecularColor *
                            glm::pow(glm::clamp(glm::dot(reflectionDirection,
                                                         viewDirection),
                                                0.f, 1.f),
                                     objectShininess);
                        color += diffuseColor + specularColor;
                    }
                }
                color += objects[minObject]->GetAmbientColor();

                color = glm::clamp(color, glm::vec3(0), glm::vec3(1));

                pixels[0 + pixelIndex] =
                    static_cast<unsigned char>(color.x * 255);
                pixels[1 + pixelIndex] =
                    static_cast<unsigned char>(color.y * 255);
                pixels[2 + pixelIndex] =
                    static_cast<unsigned char>(color.z * 255);
            }
        }
    }

    Image::ViewBMP("Render.bmp", w, h, pixels);
    delete[] pixels;
}

Scene::~Scene() {
    for (auto *object : objects) {
        delete object;
    }
}

void Scene::GenerateScene(const std::string &filename) {
    std::ifstream inputStream;
    inputStream.open(filename);

    if (inputStream.fail()) {
        std::cout << "Could not open scene file: " << filename << "."
                  << std::endl;
        return;
    }

    // Get first number which contain the number of objects.
    inputStream >> objectCount;
    // std::cout << objectCount << std::endl;

    for (int i = 0; i < objectCount; ++i) {
        std::string type;
        inputStream >> type;
        // std::cout << type;
        if (type == "camera") {
            GetCamera(inputStream);
        } else if (type == "plane") {
            GetPlane(inputStream);
        } else if (type == "sphere") {
            GetSphere(inputStream);
        } else if (type == "light") {
            GetLight(inputStream);
        } else if (type == "mesh") {
            GetMesh(inputStream);
        } else {
            std::cout << "Bad input." << std::endl;
        }
    }
};

void Scene::GetCamera(std::ifstream &inputStream) {
    // Get position.
    int px, py, pz;
    inputStream.ignore(8, ' ');
    inputStream >> px >> py >> pz;
    // Get field of view.
    int fov;
    inputStream.ignore(8, ' ');
    inputStream >> fov;
    // Get focal length.
    int f;
    inputStream.ignore(8, ' ');
    inputStream >> f;
    // Get aspect ratio.
    float a;
    inputStream.ignore(8, ' ');
    inputStream >> a;
    camera = Camera(px, py, pz, fov, f, a);
}

void Scene::GetPlane(std::ifstream &inputStream) {
    // Get normal.
    float nx, ny, nz;
    inputStream.ignore(8, ' ');
    inputStream >> nx >> ny >> nz;
    // Get position.
    int px, py, pz;
    inputStream.ignore(8, ' ');
    inputStream >> px >> py >> pz;
    // Get ambient color.
    float ax, ay, az;
    inputStream.ignore(8, ' ');
    inputStream >> ax >> ay >> az;
    // Get diffuse color.
    float dx, dy, dz;
    inputStream.ignore(8, ' ');
    inputStream >> dx >> dy >> dz;
    // Get specular color.
    float sx, sy, sz;
    inputStream.ignore(8, ' ');
    inputStream >> sx >> sy >> sz;
    // Get shininess
    float s;
    inputStream.ignore(8, ' ');
    inputStream >> s;
    objects.push_back(new Plane(nx, ny, nz, px, py, pz, ax, ay, az, dx, dy, dz,
                                sx, sy, sz, s));
}

void Scene::GetSphere(std::ifstream &inputStream) {
    // Get position.
    int px, py, pz;
    inputStream.ignore(8, ' ');
    inputStream >> px >> py >> pz;
    // Get radius.
    int rad;
    inputStream.ignore(8, ' ');
    inputStream >> rad;
    // Get ambient color.
    float ax, ay, az;
    inputStream.ignore(8, ' ');
    inputStream >> ax >> ay >> az;
    // Get diffuse color.
    float dx, dy, dz;
    inputStream.ignore(8, ' ');
    inputStream >> dx >> dy >> dz;
    // Get specular color.
    float sx, sy, sz;
    inputStream.ignore(8, ' ');
    inputStream >> sx >> sy >> sz;
    // Get shininess
    float s;
    inputStream.ignore(8, ' ');
    inputStream >> s;
    objects.push_back(
        new Sphere(px, py, pz, rad, ax, ay, az, dx, dy, dz, sx, sy, sz, s));
}

void Scene::GetLight(std::ifstream &inputStream) {
    // Get position.
    int px, py, pz;
    inputStream.ignore(8, ' ');
    inputStream >> px >> py >> pz;
    // Get ambient color.
    float ax, ay, az;
    ax = 0;
    ay = 0;
    az = 0;
    //    inputStream.ignore(8, ' ');
    //    inputStream >> ax >> ay >> az;
    // Get diffuse color.
    float dx, dy, dz;
    inputStream.ignore(8, ' ');
    inputStream >> dx >> dy >> dz;
    // Get specular color.
    float sx, sy, sz;
    inputStream.ignore(8, ' ');
    inputStream >> sx >> sy >> sz;
    lights.emplace_back(px, py, pz, ax, ay, az, dx, dy, dz, sx, sy, sz);
}

void Scene::GetMesh(std::ifstream &inputStream) {
    // Get position.
    std::string objectFilename;
    inputStream.ignore(8, ' ');
    inputStream >> objectFilename;
    // Get ambient color.
    float ax, ay, az;
    inputStream.ignore(8, ' ');
    inputStream >> ax >> ay >> az;
    // Get diffuse color.
    float dx, dy, dz;
    inputStream.ignore(8, ' ');
    inputStream >> dx >> dy >> dz;
    // Get specular color.
    float sx, sy, sz;
    inputStream.ignore(8, ' ');
    inputStream >> sx >> sy >> sz;
    // Get shininess
    float s;
    inputStream.ignore(8, ' ');
    inputStream >> s;

    std::ifstream objectFileStream;
    objectFileStream.open(objectFilename);

    if (objectFileStream.fail()) {
        std::cout << "Could not open scene file: " << objectFilename << "."
                  << std::endl;
        return;
    }

    //    std::string input;
    //    objectFileStream >> input;
    //    while (input != "default") {
    //        objectFileStream >> input;
    //    }
    //    objectFileStream >> input;
    //
    //    std::vector<glm::vec3> vertices;
    //    while (input == "v") {
    //        float px, py, pz;
    //        objectFileStream >> px >> py >> pz;
    //        vertices.emplace_back(px, py, pz);
    //        objectFileStream >> input;
    //    }
    //    assert(vertices.size() > 2);
    //
    //    std::cout << vertices.size();

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> normalIndices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    loadOBJ(objectFilename.c_str(), indices, vertices, normalIndices, normals,
            UVs);

    std::cout << indices.size() << " " << vertices.size() << " "
              << normals.size() << std::endl;

    std::vector<Triangle> triangles;
    for (int i = 0; i < indices.size(); i += 3) {
        //        std::cout << glm::to_string(normals[normalIndices[i + 0]]) <<
        //        std::endl; std::cout << glm::to_string(normals[normalIndices[i
        //        + 1]]) << std::endl; std::cout <<
        //        glm::to_string(normals[normalIndices[i + 2]]) << std::endl;

        triangles.emplace_back(
            vertices[indices[i + 0]], vertices[indices[i + 1]],
            vertices[indices[i + 2]], normals[normalIndices[i + 0]],
            normals[normalIndices[i + 1]], normals[normalIndices[i + 2]]);
    }

    objects.push_back(
        new Mesh(triangles, ax, ay, az, dx, dy, dz, sx, sy, sz, s));
}
