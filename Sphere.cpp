//
// Created by oscar on 23/03/19.
//

#include "Sphere.h"
#include "Ray.h"

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

    // float a = 1;
    float b = 2 * (pd.x * dx + pd.y * dy + pd.z * dz);
    auto c = static_cast<float>(glm::pow(dx, 2) + glm::pow(dy, 2) +
                                glm::pow(dz, 2) - glm::pow(radius, 2));
    auto discriminant = static_cast<float>(glm::pow(b, 2) - 4 * c);

    // FIXME: Need some epsilon value.
    if (discriminant < 0.001) {
        return -1;
        // return -b / 2;
    } else {
        float root = glm::sqrt(discriminant);
        return std::min((-b + root) / 2, (-b - root) / 2);
    }
}
