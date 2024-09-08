#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "Object.hpp"
#include <memory>
#include <cmath>

class Sphere : public Object {
public:
    Sphere(const Vector3& center, float radius)
            : m_center(center), m_radius(radius) {}

    bool intersect(const Ray& ray, float& t) const override {
        Vector3 oc = ray.origin() - m_center;
        float a = Vector3::dot(ray.direction(), ray.direction());
        float b = 2.0f * Vector3::dot(oc, ray.direction());
        float c = Vector3::dot(oc, oc) - m_radius * m_radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return false;
        } else {
            float temp = (-b - std::sqrt(discriminant)) / (2.0f * a);
            if (temp < 0) {
                temp = (-b + std::sqrt(discriminant)) / (2.0f * a);
            }
            if (temp < 0) {
                return false;
            }
            t = temp;
            return true;
        }
    }

    Vector3 getNormal(const Vector3& point) const override {
        return (point - m_center).normalized();
    }

    void getUV(const Vector3& point, float& u, float& v) const override {
        Vector3 normal = getNormal(point);
        u = 0.5f + std::atan2(normal.z, normal.x) / (2 * M_PI);
        v = 0.5f - std::asin(normal.y) / M_PI;
    }

    void getBoundingBox(Vector3& min, Vector3& max) const override {
        min = m_center - Vector3(m_radius, m_radius, m_radius);
        max = m_center + Vector3(m_radius, m_radius, m_radius);
    }

private:
    Vector3 m_center;
    float m_radius;
};