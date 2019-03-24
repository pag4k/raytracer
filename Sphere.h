

#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

class Sphere : public Object {
  public:
    Sphere(float px, float py, float pz, int rad, float ax, float ay, float az,
           float dx, float dy, float dz, float sx, float sy, float sz, float s);
    float GetIntersection(const Ray &ray) const override;

  private:
    glm::vec3 position; // Position of the center of the sphere
    int radius;         // Radius of the sphere
};

#endif // SPHERE_H
