#ifndef LIGHT_H
#define LIGHT_H

#include "Vec3.h"

class Light {
public:
    Vec3 position;
    Vec3 color;
    float intensity;

    Light(const Vec3& pos, const Vec3& col, float intens)
        : position(pos), color(col), intensity(intens) {}
};

#endif // LIGHT_H