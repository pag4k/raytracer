
#include "Scene.h"

#include <fstream>
#include <iostream>

#include "CImg.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"

Scene::Scene(const std::string &filename) {
    GenerateScene(filename);
    auto dimensions = camera.GetDimensions();
    const int maxh = dimensions.first;
    const int maxw = dimensions.second;

    std::cout << "Number of objects: " << objects.size() << std::endl;

    const int h = 300;
    const int w = 400;

    std::cout << "Number of pixels: " << 3 * (2 * h + 1) * (2 * w + 1)
              << std::endl;
    auto pixels =
        new std::vector<unsigned char>(3 * (2 * h + 1) * (2 * w + 1), 0);

    for (int y = -h; y <= h; ++y) {
        for (int x = -w; x <= w; ++x) {
            int wx = static_cast<int>(glm::round(maxw / w * x));
            int wy = static_cast<int>(glm::round(maxh / h * y));
            const Ray ray = camera.CreateRay(wx, wy);
            int closestObject = -1;
            float minDistance = std::numeric_limits<float>::max();
            for (int i = 0; i < objects.size(); ++i) {
                float distance = objects[i]->GetIntersection(ray);
                if (0 < distance && distance < minDistance) {
                    closestObject = i;
                    minDistance = distance;
                }
            }
            if (closestObject != -1) {
                (*pixels)[0 + 3 * ((h - y) * (2 * w + 1) + x + w)] =
                    static_cast<unsigned char>(
                        objects[closestObject]->GetColor().x * 255);
                (*pixels)[1 + 3 * ((h - y) * (2 * w + 1) + x + w)] =
                    static_cast<unsigned char>(
                        objects[closestObject]->GetColor().y * 255);
                (*pixels)[2 + 3 * ((h - y) * (2 * w + 1) + x + w)] =
                    static_cast<unsigned char>(
                        objects[closestObject]->GetColor().z * 255);
            } else {
                // std::cout << "No collisions." << std::endl;
            }
        }
    }

    std::cout << (int)(*pixels)[0] << " " << (int)(*pixels)[1] << " "
              << (int)(*pixels)[2] << " " << std::endl;

    // Creates an image with three channels and sets it to black
    cimg_library::CImg<float> image((2 * w + 1), (2 * h + 1), 1, 3, 0);
    for (int y = 0; y <= (2 * h + 1); ++y) {
        for (int x = 0; x <= (2 * w + 1); ++x) {
            image(x, y, 0) = (*pixels)[0 + 3 * (y * (2 * w + 1) + x)];
            image(x, y, 1) = (*pixels)[1 + 3 * (y * (2 * w + 1) + x)];
            image(x, y, 2) = (*pixels)[1 + 3 * (y * (2 * w + 1) + x)];
        }
    }

    //    for (int y = -h; y <= h; ++y) {
    //        for (int x = -w; x <= w; ++x) {
    //            const Ray ray = camera.CreateRay(x, y);
    //            int closestObject = -1;
    //            float minDistance = std::numeric_limits<float>::max();
    //            for (int i = 0; i < objects.size(); ++i) {
    //                float distance = objects[i]->GetIntersection(ray);
    //                if (0 < distance && distance < minDistance) {
    //                    closestObject = i;
    //                    minDistance = distance;
    //                }
    //            }
    //            if (closestObject != -1) {
    //                pixels[0 + 3 * ((y + h) * (2 * w + 1) + x + w)] =
    //                    objects[closestObject]->GetColor().x;
    //                pixels[1 + 3 * ((y + h) * (2 * w + 1) + x + w)] =
    //                    objects[closestObject]->GetColor().y;
    //                pixels[2 + 3 * ((y + h) * (2 * w + 1) + x + w)] =
    //                    objects[closestObject]->GetColor().z;
    //            }
    //        }
    //    }
    //
    //    // Creates an image with three channels and sets it to black
    //    cimg_library::CImg<float> image((2 * w + 1), (2 * h + 1), 1, 3, 0);
    //    for (int y = 0; y <= (2 * h + 1); ++y) {
    //        for (int x = 0; x <= (2 * w + 1); ++x) {
    //            image(x, y, 0) = pixels[0 + 3 * (y * (2 * w + 1) + x)] *
    //            255.0f; image(x, y, 1) = pixels[1 + 3 * (y * (2 * w + 1) + x)]
    //            * 255.0f; image(x, y, 2) = pixels[1 + 3 * (y * (2 * w + 1) +
    //            x)] * 255.0f;
    //        }
    //    }

    // Save out the image in BMP format. Pixel values must be in the range
    // [0,255]
    // delete pixels;

    image.save("render.bmp");
    // Display the rendered image on screen
    cimg_library::CImgDisplay main_disp(image, "Render");
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
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
    //    planes.emplace_back(px, py, pz, nx, ny, nz, ax, ay, az, dx, dy,
    //    dz, sx, sy,
    //                        sz, s);
}

void Scene::GetSphere(std::ifstream &inputStream) {
    // Get position.
    int px, py, pz;
    inputStream.ignore(8, ' ');
    inputStream >> px >> py >> pz;
    // Get radius.
    float rad;
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
    //    spheres.emplace_back(px, py, pz, rad, ax, ay, az, dx, dy, dz, sx,
    //    sy, sz,
    //                         s);
}

void Scene::GetLight(std::ifstream &inputStream) {
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
    lights.emplace_back(px, py, pz, ax, ay, az, dx, dy, dz, sx, sy, sz);
}
