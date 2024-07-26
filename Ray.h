// Ray.h
#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
    Vec3 origin;
    Vec3 direction;

    Ray() : origin(Vec3()), direction(Vec3()) {}


    Ray(const Vec3& origin, const Vec3& direction)
            : origin(origin), direction(direction) {}

    Vec3 point_at_parameter(float t) const {
        return origin + direction * t;
    }
};

#endif // RAY_H