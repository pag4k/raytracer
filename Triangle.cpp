
#include <cassert>
#include <glm/gtx/string_cast.hpp>
//#include <iostream>

#include "Plane.h"
#include "Ray.h"
#include "Triangle.h"

Triangle::Triangle(glm::vec3 vertexPosition1, glm::vec3 vertexPosition2,
                   glm::vec3 vertexPosition3, glm::vec3 vertexNormal1,
                   glm::vec3 vertexNormal2, glm::vec3 vertexNormal3)
    : vertexPositions{vertexPosition1, vertexPosition2, vertexPosition3},
      vertexNormals{vertexNormal1, vertexNormal2, vertexNormal3},
      p0(vertexPositions[1] - vertexPositions[0]),
      p1(vertexPositions[2] - vertexPositions[1]),
      p2(vertexPositions[0] - vertexPositions[2]),
      area(0.5 * glm::length(glm::cross(p0, p1))),
      normal(glm::normalize(glm::cross(p0, p1))) {
    assert(area > 0);
    //    std::cout << glm::length(vertexNormal1 - vertexNormal2) << std::endl;
    //    std::cout << glm::length(vertexNormal2 - vertexNormal3) << std::endl;
    //    std::cout << glm::length(vertexNormal3 - vertexNormal1) << std::endl;
    //    std::cout << glm::to_string(vertexNormal1);
    //    std::cout << glm::to_string(vertexNormal2);
    //    std::cout << glm::to_string(vertexNormal3);
    assert(vertexNormal1 == vertexNormal2);
    assert(vertexNormal2 == vertexNormal3);
}

float Triangle::GetIntersection(const Ray &ray) const {
    float denominator = glm::dot(ray.GetDir(), normal);
    // FIXME: Need some epsilon value.
    if (glm::abs(denominator) < 0.0001) {
        return -1;
    } else {
        float distance =
            glm::dot(vertexPositions[0] - ray.GetOrigin(), normal) /
            denominator;
        glm::vec3 intersection = ray.GetPoint(distance);

        glm::vec3 c0 = vertexPositions[0] - intersection;
        glm::vec3 c1 = vertexPositions[1] - intersection;
        glm::vec3 c2 = vertexPositions[2] - intersection;

        float alpha = glm::abs(0.5 * glm::length(glm::cross(c0, p0)) / area);
        // assert(alpha > 0);
        float beta = glm::abs(0.5 * glm::length(glm::cross(c1, p1)) / area);
        // assert(beta > 0);
        float gamma = glm::abs(0.5 * glm::length(glm::cross(c2, p2)) / area);
        // assert(gamma > 0);

        if (alpha <= 1 && beta <= 1 && gamma <= 1 &&
            glm::abs(alpha + beta + gamma - 1) < 0.0001) {
            //            std::cout << "alpha " << alpha << "beta " << beta <<
            //            "gamma "
            //                      << gamma << std::endl;
            return distance;
        } else {
            return -1;
        }
    }
}

glm::vec3 Triangle::GetNormal(const Ray &ray, float distance) const {
    return vertexNormals[0];
}