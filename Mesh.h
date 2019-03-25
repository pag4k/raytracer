

#ifndef COMP371RAYTRACER_MESH_H
#define COMP371RAYTRACER_MESH_H

#include "Object.h"
#include "Triangle.h"

#include <vector>

class Mesh : public Object {
  public:
    Mesh(const std::vector<Triangle> &triangleVector, float ax, float ay,
         float az, float dx, float dy, float dz, float sx, float sy, float sz,
         float s);
    ~Mesh();
    float GetIntersection(const Ray &ray) const override;
    glm::vec3 GetNormal(const Ray &ray, float distance) const override;

  private:
    Triangle *triangles;
    int triangleCount;
    mutable int triangleIndex = -1;
};

#endif // MESH_H