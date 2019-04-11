
#include <cassert>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Common.h"
#include "Plane.h"
#include "Ray.h"
#include "Triangle.h"

Triangle::Triangle(glm::vec3 vertexPosition1, glm::vec3 vertexPosition2,
                   glm::vec3 vertexPosition3)
    : vertexPositions{vertexPosition1, vertexPosition2, vertexPosition3},
      p0(vertexPositions[1] - vertexPositions[0]),
      p1(vertexPositions[2] - vertexPositions[1]),
      p2(vertexPositions[0] - vertexPositions[2]),
      doubleArea(glm::length(glm::cross(p0, p1))),
      normal(glm::normalize(glm::cross(p0, p1))) {}

float Triangle::GetIntersection(const Ray &ray) const {
    const float denominator = glm::dot(ray.GetDir(), normal);
    // Back-face culling.
    if (denominator > EPSILON) {
        return -1;
        // Parallel
    } else if (glm::abs(denominator) < EPSILON) {
        return -1;
    } else {
        const float distance =
            glm::dot(vertexPositions[0] - ray.GetOrigin(), normal) /
            denominator;

        // If distance is negative, abort.
        if (distance < EPSILON) {
            return -1;
        }

        const glm::vec3 intersection = ray.GetPoint(distance);
        const glm::vec3 c0 = vertexPositions[0] - intersection;
        const glm::vec3 c1 = vertexPositions[1] - intersection;
        const glm::vec3 c2 = vertexPositions[2] - intersection;

        // Compute areas.
        const float doubleAlpha = glm::length(glm::cross(c0, p0));
        const float doubleBeta = glm::length(glm::cross(c1, p1));
        const float doubleGamma = glm::length(glm::cross(c2, p2));

        // Compare areas.
        if (glm::abs(doubleAlpha + doubleBeta + doubleGamma - doubleArea) <
            1000 * EPSILON) {
            return distance;
        } else {
            return -1;
        }
    }
}

glm::vec3 Triangle::GetNormal() const { return normal; }