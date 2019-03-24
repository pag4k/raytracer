
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
  public:
    Light(float px, float py, float pz, float ax, float ay, float az, float dx,
          float dy, float dz, float sx, float sy, float sz);

  private:
    glm::vec3 position;       // Position of the light
    glm::vec3 ambient_color;  // Ambient color of the light
    glm::vec3 diffuse_color;  // Diffuse color of the light
    glm::vec3 specular_color; // Specular color of the light
};

#endif // LIGHT_H
