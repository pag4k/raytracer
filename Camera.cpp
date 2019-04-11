
#include "Camera.h"

// Assume the camera is pointing toward the negative z-axis.
Camera::Camera(int px, int py, int pz, float fov, int f, float a)
    : position(px, py, pz), direction(0, 0, -1), fov(glm::radians(fov)),
      focal_length(f), aspect_ratio(a) {}

std::pair<float, float> Camera::GetDimensions() {
    const float h = glm::abs(focal_length * glm::tan(fov / 2));
    const float w = aspect_ratio * h;
    return std::pair(h, w);
}

Ray Camera::CreateRay(int x, int y) {
    return Ray(position,
               glm::normalize(static_cast<float>(focal_length) * direction +
                              glm::vec3(x, y, 0)));
}
