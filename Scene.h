
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "Light.h"

class Camera;
class Object;

class Scene {
  public:
    explicit Scene(const std::string &filename);
    ~Scene();
    void Render(const std::string &filename);

  private:
    void GetCamera(std::ifstream &inputStream);
    void GetPlane(std::ifstream &inputStream);
    void GetSphere(std::ifstream &inputStream);
    void GetLight(std::ifstream &inputStream);
    void GetMesh(std::ifstream &inputStream);

    Camera *camera = nullptr;      // Pointer to only camera
    std::vector<Light> lights;     // All lights.
    std::vector<Object *> objects; // All objects: planes, spheres, meshes...
};

#endif // SCENE_H
