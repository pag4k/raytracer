
#include "Light.h"

Light::Light(float px, float py, float pz, float ax, float ay, float az,
             float dx, float dy, float dz, float sx, float sy, float sz)
    : position(px, py, pz), ambientColor(ax, ay, az), diffuseColor(dx, dy, dz),
      specularColor(sx, sy, sz) {}

Ray Light::CreateRay(const glm::vec3 &origin) {
    return Ray(origin, glm::normalize(position - origin));
}