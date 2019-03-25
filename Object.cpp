
#include "Object.h"

Object::Object(float ax, float ay, float az, float dx, float dy, float dz,
               float sx, float sy, float sz, float s)
    : ambientColor(ax, ay, az), diffuseColor(dx, dy, dz),
      specularColor(sx, sy, sz), shininess(s) {}
