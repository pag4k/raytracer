
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "Camera.h"
#include "Light.h"
#include "Object.h"

#include "Plane.h"
#include "Sphere.h"

class Scene {
  public:
    explicit Scene(const std::string &filename);
    ~Scene();

  private:
    void GenerateScene(const std::string &filename);
    void GetCamera(std::ifstream &inputStream);
    void GetPlane(std::ifstream &inputStream);
    void GetSphere(std::ifstream &inputStream);
    void GetLight(std::ifstream &inputStream);

    int objectCount;
    Camera camera;
    // MESH!
    std::vector<Light> lights;
    std::vector<Object *> objects;
    // std::vector<Plane> planes;
    // std::vector<Sphere> spheres;

    // std::vector<float> pixels;
};

#endif // SCENE_H
