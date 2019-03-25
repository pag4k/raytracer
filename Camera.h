

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Ray.h"

class Camera {
  public:
    Camera() = default;
    Camera(int px, int py, int pz, float fov, int f, float a);
    float GetAspectRatio() const { return aspect_ratio; };
    std::pair<float, float> GetDimensions();
    Ray CreateRay(int x, int y);

  private:
    glm::vec3 position;  // Location of the camera
    glm::vec3 direction; // Direction of the camera
    float fov;           // Field-of-view in degrees)
    int focal_length;    // Focal length of the camera
    float aspect_ratio;  // Aspect ratio of the camera
};

#endif // CAMERA_H
