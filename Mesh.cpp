
#include "Mesh.h"

Mesh::Mesh(const std::vector<Triangle> &triangleVector, float ax, float ay,
           float az, float dx, float dy, float dz, float sx, float sy, float sz,
           float s)
    : triangleCount(static_cast<int>(triangleVector.size())),
      Object(ax, ay, az, dx, dy, dz, sx, sy, sz, s) {

    triangles = new Triangle[triangleCount];
    std::copy(triangleVector.begin(), triangleVector.end(), triangles);
}

Mesh::~Mesh() { delete[] triangles; }

float Mesh::GetIntersection(const Ray &ray) const {
    int closestObject = -1;
    float minDistance = std::numeric_limits<float>::max();
    for (int i = 0; i < triangleCount; ++i) {
        float distance = triangles[i].GetIntersection(ray);
        if (0 < distance && distance < minDistance) {
            closestObject = i;
            minDistance = distance;
        }
    }
    if (closestObject == -1) {
        return -1;
    } else {
        triangleIndex = closestObject;
        return minDistance;
    }
}
glm::vec3 Mesh::GetNormal(const Ray &ray, float distance) const {
    return triangles[triangleIndex].GetNormal(ray, distance);
}