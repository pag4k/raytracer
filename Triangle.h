
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"

#include <glm/glm.hpp>

class Triangle {
  public:
    Triangle() = default;
    Triangle(glm::vec3 vertexPosition1, glm::vec3 vertexPosition2,
             glm::vec3 vertexPosition3, glm::vec3 vertexNormal1,
             glm::vec3 vertexNormal2, glm::vec3 vertexNormal3);
    float GetIntersection(const Ray &ray) const;
    glm::vec3 GetNormal(const Ray &ray, float distance) const;

  private:
    glm::vec3 vertexNormals[3];   // Normal of the place
    glm::vec3 vertexPositions[3]; // Position of a point on the plane
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 p2;
    float area;
    glm::vec3 normal;
};

#endif // TRIANGLE_H
