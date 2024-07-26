// Vec3.h
#ifndef VEC3_H
#define VEC3_H

#include <cmath>

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float f) const { return Vec3(x * f, y * f, z * f); }
    Vec3 operator/(float f) const { return Vec3(x / f, y / f, z / f); }

    Vec3& normalize() {
        float mag = magnitude();
        if (mag > 0) {
            *this = *this / mag;
        }
        return *this;
    }

    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    float magnitude() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * 2 * this->dot(normal);
    }
    static Vec3 random() {
        return Vec3(drand48(), drand48(), drand48());
    }
    static Vec3 random(float min, float max) {
        return Vec3(drand48() * (max - min) + min,
                    drand48() * (max - min) + min,
                    drand48() * (max - min) + min);
    }
    Vec3 normalize() const {
        float mag = magnitude();
        if (mag > 0) {
            return *this / mag;
        }
        return *this;
    }
};

#endif // VEC3_H