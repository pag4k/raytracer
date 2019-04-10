

#ifndef COMP371RAYTRACER_MESH_H
#define COMP371RAYTRACER_MESH_H

#include "Object.h"
#include "Triangle.h"

#include <vector>

class Mesh : public Object {
  public:
    Mesh() = delete;
    Mesh(const std::vector<Triangle> &triangleVector, float ax, float ay,
         float az, float dx, float dy, float dz, float sx, float sy, float sz,
         float s);
    ~Mesh() override;
    Mesh(const Mesh &) = delete;
    void operator=(const Mesh &) = delete;
    float GetIntersection(const Ray &ray) const override;
    glm::vec3 GetNormal(const Ray &ray, float distance) const override;

  private:
    Triangle *triangles;            // Pointer to array of triangles.
    int triangleCount;              // Number of triangles.
    mutable int triangleIndex = -1; // The index of the intersected triangle.
};

#endif // MESH_H
