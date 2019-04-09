
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

using namespace glm;

Scene::Scene(const std::string &filename) : filename(filename) {
    std::cout << "Parsing scene: " << filename << ".txt" << std::endl;
    std::ifstream inputStream;
    inputStream.open(filename + ".txt");

    if (inputStream.fail()) {
        std::cout << "Could not open scene file: " << filename << ".txt."
                  << std::endl;
        return;
    }
    // Get first number which contain the number of objects.
    int objectCount;
    inputStream >> objectCount;

    for (int i = 0; i < objectCount; ++i) {
        std::string type;
        inputStream >> type;
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
}

Scene::~Scene() {
    delete camera;
    for (auto *object : objects) {
        delete object;
    }
}

void Scene::Render() {
    const auto dimensions = camera->GetDimensions();
    const int h = static_cast<int>(dimensions.first);
    const int w = static_cast<int>(dimensions.second);
    const int pixelCount = (2 * h) * (2 * w);

    std::cout << "Width: " << (2 * w) << " Height: " << (2 * h);
    std::cout << " Number of pixels: " << pixelCount;
    std::cout << " Number of objects: " << objects.size();
    std::cout << " Number of lights: " << lights.size() << std::endl;

    auto *const pixels = new unsigned char[3 * pixelCount];

    for (int y = -h; y < h; ++y) {
        for (int x = -w; x < w; ++x) {
            const int pixelIndex = 3 * ((h - 1 - y) * (2 * w) + x + w);
            const Ray ray = camera->CreateRay(x, y);
            int minObj = -1;
            float minDistance = std::numeric_limits<float>::max();
            for (int i = 0; i < objects.size(); ++i) {
                const float distance = objects[i]->GetIntersection(ray);
                if (0 < distance && distance < minDistance) {
                    minObj = i;
                    minDistance = distance;
                }
            }
            vec3 color(0);
            if (minObj != -1) {
                const vec3 normal =
                    normalize(objects[minObj]->GetNormal(ray, minDistance));
                const vec3 objDiffColor = objects[minObj]->GetDiffColor();
                const vec3 objSpecColor = objects[minObj]->GetSpecColor();
                const float objShininess = objects[minObj]->GetShininess();
                for (auto light : lights) {
                    const Ray shadowRay =
                        light.CreateRay(ray.GetPoint(minDistance - 0.01f));
                    bool unblocked = true;
                    for (auto obj : objects) {
                        float distance = obj->GetIntersection(shadowRay);
                        if (0 < distance) {
                            unblocked = false;
                            break;
                        }
                    }

                    if (unblocked) {
                        const vec3 viewDir = -ray.GetDir();
                        const vec3 lightDir = shadowRay.GetDir();
                        const vec3 reflDir = normalize(
                            2 * dot(lightDir, normal) * normal - lightDir);

                        const vec3 diffColor =
                            light.GetDiffColor() * objDiffColor *
                            clamp(dot(lightDir, normal), 0.f, 1.f);
                        const vec3 specColor =
                            light.GetSpecColor() * objSpecColor *
                            pow(clamp(dot(reflDir, viewDir), 0.f, 1.f),
                                objShininess);
                        color += diffColor + specColor;
                    }
                }
                color += objects[minObj]->GetAmbtColor();

                color = clamp(color, vec3(0), vec3(1));
            }
            pixels[0 + pixelIndex] = static_cast<unsigned char>(color.x * 255);
            pixels[1 + pixelIndex] = static_cast<unsigned char>(color.y * 255);
            pixels[2 + pixelIndex] = static_cast<unsigned char>(color.z * 255);
        }
    }

    std::cout << "Generating: " << filename << ".bmp" << std::endl;
    Image::ViewBMP(filename + ".bmp", w, h, pixels);
    delete[] pixels;
}

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
    camera = new Camera(px, py, pz, fov, f, a);
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
    //    std::vector<vec3> vertices;
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
    std::vector<vec3> vertices;
    std::vector<unsigned int> normalIndices;
    std::vector<vec3> normals;
    std::vector<vec2> UVs;
    loadOBJ(objectFilename.c_str(), indices, vertices, normalIndices, normals,
            UVs);

    //    std::cout << indices.size() << " " << vertices.size() << " "
    //              << normals.size() << std::endl;

    std::vector<Triangle> triangles;
    for (int i = 0; i < indices.size(); i += 3) {
        //        std::cout << to_string(normals[normalIndices[i + 0]]) <<
        //        std::endl; std::cout << to_string(normals[normalIndices[i
        //        + 1]]) << std::endl; std::cout <<
        //        to_string(normals[normalIndices[i + 2]]) << std::endl;

        triangles.emplace_back(
            vertices[indices[i + 0]], vertices[indices[i + 1]],
            vertices[indices[i + 2]], normals[normalIndices[i + 0]],
            normals[normalIndices[i + 1]], normals[normalIndices[i + 2]]);
    }

    objects.push_back(
        new Mesh(triangles, ax, ay, az, dx, dy, dz, sx, sy, sz, s));
}
