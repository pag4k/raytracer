
#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

class Plane : public Object {
  public:
    Plane(float nx, float ny, float nz, float px, float py, float pz, float ax,
          float ay, float az, float dx, float dy, float dz, float sx, float sy,
          float sz, float s);
    float GetIntersection(const Ray &ray) const override;
    glm::vec3 GetNormal(const Ray &ray, float distance) const override;

  private:
    glm::vec3 normal;   // Normal of the place
    glm::vec3 position; // Position of a point on the plane
};

#endif // PLANE_H
