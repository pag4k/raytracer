
#include "Plane.h"
#include "Ray.h"

Plane::Plane(float nx, float ny, float nz, float px, float py, float pz,
             float ax, float ay, float az, float dx, float dy, float dz,
             float sx, float sy, float sz, float s)
    : normal(nx, ny, nz), position(px, py, pz),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {}

float Plane::GetIntersection(const Ray &ray) const {
    float denominator = glm::dot(ray.GetDirection(), normal);

    // FIXME: Need some epsilon value.
    if (glm::abs(denominator) < 0.0001) {
        return -1;
    } else {
        // Back-facing are not considered since the return value will be
        // negative in th case.
        return glm::dot(position - ray.GetOrigin(), normal) / denominator;
    }
}

glm::vec3 Plane::GetNormal(const Ray &ray, float distance) const {
    return normal;
}