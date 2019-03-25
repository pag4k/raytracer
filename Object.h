
#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <optional>

class Ray;

class Object {
  public:
    Object(float ax, float ay, float az, float dx, float dy, float dz, float sx,
           float sy, float sz, float s);
    virtual ~Object() = default;
    virtual float GetIntersection(const Ray &ray) const = 0;
    virtual glm::vec3 GetNormal(const Ray &ray, float distance) const = 0;
    const glm::vec3 &GetAmbientColor() const { return ambientColor; }
    const glm::vec3 &GetDiffuseColor() const { return diffuseColor; }
    const glm::vec3 &GetSpecularColor() const { return specularColor; }
    float GetShininess() const { return shininess; };

  private:
    glm::vec3 ambientColor;  // Ambient color of the object
    glm::vec3 diffuseColor;  // Diffuse color of the object`
    glm::vec3 specularColor; // Specular color of the object
    float shininess;         // Specular factor of the object
};

#endif // OBJECT_H
