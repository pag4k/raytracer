
#include "Light.h"

Light::Light(float px, float py, float pz, float ax, float ay, float az,
             float dx, float dy, float dz, float sx, float sy, float sz)
    : position(px, py, pz), ambient_color(ax, ay, az),
      diffuse_color(dx, dy, dz), specular_color(sx, sy, sz) {}