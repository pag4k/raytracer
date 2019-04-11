
#include "Sphere.h"
#include "Common.h"
#include "Ray.h"

Sphere::Sphere(float px, float py, float pz, int rad, float ax, float ay,
               float az, float dx, float dy, float dz, float sx, float sy,
               float sz, float s)
    : position(px, py, pz), radius(rad),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {}

float Sphere::GetIntersection(const Ray &ray) const {
    const glm::vec3 &p0 = ray.GetOrigin();
    const glm::vec3 &pd = ray.GetDir();
    const float dx = p0.x - position.x;
    const float dy = p0.y - position.y;
    const float dz = p0.z - position.z;

    const float c = dx * dx + dy * dy + dz * dz - radius * radius;
    // Back-face culling if inside the sphere.
    if (c < EPSILON) {
        return -1;
    }
    const float b = 2 * (pd.x * dx + pd.y * dy + pd.z * dz);
    const float discriminant = b * b - 4 * c;

    if (discriminant < -EPSILON) {
        return -1;
    } else if (glm::abs(discriminant) < EPSILON) {
        return -b / 2;
    } else {
        const float root = glm::sqrt(discriminant);
        const float zero1 = (-b + root) / 2;
        const float zero2 = (-b - root) / 2;
        if (zero1 < EPSILON && zero2 < EPSILON) {
            return -1;
        } else if (zero1 < EPSILON) {
            return zero2;
        } else if (zero2 < EPSILON) {
            return zero1;
        } else {
            return std::min(zero1, zero2);
        }
    }
}

glm::vec3 Sphere::GetNormal(const Ray &ray, float distance) const {
    const glm::vec3 intersection = ray.GetPoint(distance);
    return glm::normalize(intersection -
                          position); // / static_cast<float>(radius);
}