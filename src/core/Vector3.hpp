#pragma once
#include <cmath>

class Vector3 {
public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    float length() const { return std::sqrt(x*x + y*y + z*z); }
    float lengthSquared() const { return x*x + y*y + z*z; }
    Vector3 normalized() const { return *this / length(); }

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

    float getAxis(int axis) const {
        switch(axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Invalid axis");
        }
    }

    static float dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector3 cross(const Vector3& a, const Vector3& b) {
        return Vector3(a.y * b.z - a.z * b.y,
                       a.z * b.x - a.x * b.z,
                       a.x * b.y - a.y * b.x);
    }

    float x, y, z;
};

inline Vector3 operator*(float s, const Vector3& v) {
    return v * s;
}