#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "Object.hpp"
#include <memory>
#include <limits>

class Plane : public Object {
public:
    Plane(const Vector3& position, const Vector3& normal)
            : m_position(position), m_normal(normal.normalized()) {
        // Create a coordinate system for UV mapping
        m_u = Vector3::cross(m_normal, Vector3(0, 1, 0)).normalized();
        if (m_u.length() < 0.1f) {
            m_u = Vector3::cross(m_normal, Vector3(1, 0, 0)).normalized();
        }
        m_v = Vector3::cross(m_normal, m_u);
    }

    bool intersect(const Ray& ray, float& t) const override {
        float denom = Vector3::dot(m_normal, ray.direction());
        if (std::abs(denom) > 1e-6) {
            Vector3 p0l0 = m_position - ray.origin();
            t = Vector3::dot(p0l0, m_normal) / denom;
            return t >= 0;
        }
        return false;
    }

    Vector3 getNormal(const Vector3& point) const override {
        return m_normal;
    }

    void getUV(const Vector3& point, float& u, float& v) const override {
        Vector3 p = point - m_position;
        u = Vector3::dot(p, m_u);
        v = Vector3::dot(p, m_v);
    }

    void getBoundingBox(Vector3& min, Vector3& max) const override {
        // Planes are infinite, so we use a large bounding box
        float inf = std::numeric_limits<float>::infinity();
        min = Vector3(-inf, -inf, -inf);
        max = Vector3(inf, inf, inf);
    }

private:
    Vector3 m_position;
    Vector3 m_normal;
    Vector3 m_u, m_v; // For UV mapping
};