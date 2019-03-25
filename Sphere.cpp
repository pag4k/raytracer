//
// Created by oscar on 23/03/19.
//

#include "Sphere.h"
#include "Ray.h"

#include <iostream>

Sphere::Sphere(float px, float py, float pz, int rad, float ax, float ay,
               float az, float dx, float dy, float dz, float sx, float sy,
               float sz, float s)
    : position(px, py, pz), radius(rad),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {}

float Sphere::GetIntersection(const Ray &ray) const {
    const glm::vec3 &p0 = ray.GetOrigin();
    const glm::vec3 &pd = ray.GetDirection();
    float dx = p0.x - position.x;
    float dy = p0.y - position.y;
    float dz = p0.z - position.z;

    // Check if back-facing.
    if (dx * dx + dy * dy + dz * dz <= radius * radius) {
        return -1;
    }

    // float a = 1;
    float b = 2 * (pd.x * dx + pd.y * dy + pd.z * dz);
    auto c = static_cast<float>(glm::pow(dx, 2) + glm::pow(dy, 2) +
                                glm::pow(dz, 2) - glm::pow(radius, 2));
    auto discriminant = static_cast<float>(glm::pow(b, 2) - 4 * c);

    // FIXME: Need some epsilon value.
    if (discriminant < 0.0001) {
        return -1;
        // return -b / 2;
    } else {
        float root = glm::sqrt(discriminant);
        float zero1 = (-b + root) / 2;
        float zero2 = (-b - root) / 2;
        if (zero1 <= 0.0001) {
            return zero2;
        } else if (zero2 <= 0.0001) {
            return zero1;
        } else {
            return std::min(zero1, zero2);
        }
    }
}

glm::vec3 Sphere::GetNormal(const Ray &ray, float distance) const {
    glm::vec3 intersection = ray.GetPoint(distance);
    return (intersection - position) / static_cast<float>(radius);
}