
#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <optional>

class Ray;

class Object {
  public:
    Object(float ax, float ay, float az, float dx, float dy, float dz, float sx,
           float sy, float sz, float s);
    virtual float GetIntersection(const Ray &ray) const = 0;
    const glm::vec3 &GetColor() const { return diffuse_color; }

  private:
    glm::vec3 ambient_color;  // Ambient color of the object
    glm::vec3 diffuse_color;  // Diffuse color of the object`
    glm::vec3 specular_color; // Specular color of the object
    float shininess;          // Specular factor of the object
};

#endif // OBJECT_H
