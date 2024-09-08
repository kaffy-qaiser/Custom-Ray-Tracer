#pragma once
#include "Vector3.hpp"

class Ray {
public:
    Ray(const Vector3& origin, const Vector3& direction)
            : m_origin(origin), m_direction(direction.normalized()) {}

    Vector3 origin() const { return m_origin; }
    Vector3 direction() const { return m_direction; }
    Vector3 pointAtParameter(float t) const { return m_origin + t * m_direction; }

private:
    Vector3 m_origin;
    Vector3 m_direction;
};
