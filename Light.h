
#ifndef LIGHT_H
#define LIGHT_H

#include "Ray.h"
#include <glm/glm.hpp>

class Light {
  public:
    Light(float px, float py, float pz, float ax, float ay, float az, float dx,
          float dy, float dz, float sx, float sy, float sz);
    const glm::vec3 &GetPos() const { return position; }
    const glm::vec3 &GetDiffColor() const { return diffuseColor; }
    const glm::vec3 &GetSpecColor() const { return specularColor; }
    Ray CreateRay(const glm::vec3 &origin);

  private:
    glm::vec3 position;      // Position of the light
    glm::vec3 ambientColor;  // Ambient color of the light
    glm::vec3 diffuseColor;  // Diffuse color of the light
    glm::vec3 specularColor; // Specular color of the light
};

#endif // LIGHT_H
