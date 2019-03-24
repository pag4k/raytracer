
#include "Object.h"

Object::Object(float ax, float ay, float az, float dx, float dy, float dz,
               float sx, float sy, float sz, float s)
    : ambient_color(ax, ay, az), diffuse_color(dx, dy, dz),
      specular_color(sx, sy, sz), shininess(s) {}