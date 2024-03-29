
#include "Scene.h"
#include <cassert>
//#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>

#include "IndexObjLoader.h"

#include "Camera.h"
#include "Common.h"
#include "Image.h"
#include "Mesh.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

using namespace glm;

Scene::Scene(const std::string &filename) {
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

void Scene::Render(const std::string &filename) {
    const auto dimensions = camera->GetDimensions();
    const int h = static_cast<int>(dimensions.first);
    const int w = static_cast<int>(dimensions.second);
    const int pixelCount = (2 * h) * (2 * w);

    std::cout << "Width: " << (2 * w) << " Height: " << (2 * h);
    std::cout << " Number of pixels: " << pixelCount;
    std::cout << " Number of objects: " << objects.size();
    std::cout << " Number of lights: " << lights.size() << std::endl;

    auto *const pixels = new unsigned char[3 * pixelCount];

    // Iterate over all pixels.
    for (int y = -h; y < h; ++y) {
        for (int x = -w; x < w; ++x) {
            // Send ray.
            const Ray ray = camera->CreateRay(x, y);
            // Set initial closest object index and minimum distance.
            int minObj = -1;
            float minDistance = std::numeric_limits<float>::max();
            // Find closest interestion.
            for (int i = 0; i < objects.size(); ++i) {
                const float distance = objects[i]->GetIntersection(ray);
                if (EPSILON < distance && distance < minDistance) {
                    minObj = i;
                    minDistance = distance;
                }
            }
            // Set default color to black.
            vec3 color(0);
            // If there was a collision...
            if (minObj != -1) {
                const vec3 intersection =
                    ray.GetPoint(minDistance - 1000.0 * EPSILON);
                // Get normal at intersection.
                const vec3 normal =
                    normalize(objects[minObj]->GetNormal(ray, minDistance));
                // Get colors needed for Phong illumination.
                const vec3 objDiffColor = objects[minObj]->GetDiffColor();
                const vec3 objSpecColor = objects[minObj]->GetSpecColor();
                const float objShininess = objects[minObj]->GetShininess();

                // For each light...
                for (auto light : lights) {
                    // Send a shadow ray.
                    const Ray shadowRay = light.CreateRay(intersection);
                    // Check if it is blocked
                    bool unblocked = true;
                    for (auto obj : objects) {
                        float distance = obj->GetIntersection(shadowRay);
                        if (distance > EPSILON) {
                            unblocked = false;
                            break;
                        }
                    }
                    // If not blocked, compute color.
                    if (unblocked) {
                        const vec3 viewDir = -ray.GetDir();
                        const vec3 lightDir = shadowRay.GetDir();
                        const vec3 refDir = normalize(
                            2 * dot(lightDir, normal) * normal - lightDir);

                        const vec3 diffColor =
                            light.GetDiffColor() * objDiffColor *
                            clamp(dot(lightDir, normal), 0.f, 1.f);
                        const vec3 specColor =
                            light.GetSpecColor() * objSpecColor *
                            pow(clamp(dot(refDir, viewDir), 0.f, 1.f),
                                objShininess);
                        color += diffColor + specColor;
                    }
                }
                // Add ambient object color and clamp.
                color += objects[minObj]->GetAmbColor();
                color = clamp(color, vec3(0), vec3(1));
            }
            // Put color into main array.
            const int pixelIndex = 3 * ((h - 1 - y) * (2 * w) + x + w);
            pixels[0 + pixelIndex] = static_cast<unsigned char>(color.x * 255);
            pixels[1 + pixelIndex] = static_cast<unsigned char>(color.y * 255);
            pixels[2 + pixelIndex] = static_cast<unsigned char>(color.z * 255);
        }
    }

    std::cout << "Generating: " << filename << ".bmp" << std::endl;
    Image::SaveBMP(filename + ".bmp", w, h, pixels);
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

    std::vector<unsigned int> indices;
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> UVs;

    loadOBJ(objectFilename.c_str(), indices, vertices, normals, UVs);

    std::vector<Triangle> triangles;
    for (int i = 0; i < indices.size(); i += 3) {
        triangles.emplace_back(vertices[indices[i + 0]],
                               vertices[indices[i + 1]],
                               vertices[indices[i + 2]]);
    }

    objects.push_back(
        new Mesh(triangles, ax, ay, az, dx, dy, dz, sx, sy, sz, s));
}
