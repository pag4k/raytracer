
#include "Plane.h"
#include "Ray.h"

#include <iostream>

Plane::Plane(float nx, float ny, float nz, float px, float py, float pz,
             float ax, float ay, float az, float dx, float dy, float dz,
             float sx, float sy, float sz, float s)
    : normal(nx, ny, nz), position(px, py, pz),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {}

float Plane::GetIntersection(const Ray &ray) const {
    float denominator = glm::dot(ray.GetDirection(), normal);
    // FIXME: Need some epsilon value.
    if (glm::abs(denominator) < 0.001) {
        return -1;
    } else {
        return glm::dot(position - ray.GetOrigin(), normal) / denominator;
    }
}
