
#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
  public:
    Ray(glm::vec3 origin, glm::vec3 direction);
    const glm::vec3 &GetOrigin() const { return origin; };
    const glm::vec3 &GetDir() const { return direction; };
    const glm::vec3 GetPoint(float t) const { return origin + t * direction; }

  private:
    glm::vec3 origin;
    glm::vec3 direction;
};

#endif // RAY_H
