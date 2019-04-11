
#include "Plane.h"
#include "Common.h"
#include "Ray.h"

Plane::Plane(float nx, float ny, float nz, float px, float py, float pz,
             float ax, float ay, float az, float dx, float dy, float dz,
             float sx, float sy, float sz, float s)
    : normal(nx, ny, nz), position(px, py, pz),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {}

float Plane::GetIntersection(const Ray &ray) const {
    const float denominator = glm::dot(ray.GetDir(), normal);

    // Back-face culling.
    if (denominator > EPSILON) {
        return -1;
        // Parallel plane
    } else if (glm::abs(denominator) < EPSILON) {
        return -1;
    } else {
        return glm::dot(position - ray.GetOrigin(), normal) / denominator;
    }
}

glm::vec3 Plane::GetNormal(const Ray &ray, float distance) const {
    return normal;
}