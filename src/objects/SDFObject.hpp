#pragma once

#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "Object.hpp"
#include <functional>
#include <memory>
#include <algorithm>
#include <cmath>

class SDFObject : public Object {
public:
    using SDFunction = std::function<float(const Vector3&)>;

    SDFObject(SDFunction sdf, float epsilon = 0.001f)
            : m_sdf(sdf), m_epsilon(epsilon) {}

    bool intersect(const Ray& ray, float& t) const override {
        float t0 = 0.0f;
        float t1 = 1000.0f; // Max distance

        for (int i = 0; i < 100; ++i) { // Max iterations
            t = (t0 + t1) / 2.0f;
            Vector3 p = ray.pointAtParameter(t);
            float d = m_sdf(p);

            if (std::abs(d) < m_epsilon) {
                return true;
            }

            if (d > 0) {
                t0 = t;
            } else {
                t1 = t;
            }

            if (t1 - t0 < m_epsilon) {
                return false;
            }
        }

        return false;
    }

    Vector3 getNormal(const Vector3& p) const override {
        float d = m_epsilon;
        Vector3 n(
        m_sdf(Vector3(p.x + d, p.y, p.z)) - m_sdf(Vector3(p.x - d, p.y, p.z)),
                m_sdf(Vector3(p.x, p.y + d, p.z)) - m_sdf(Vector3(p.x, p.y - d, p.z)),
                m_sdf(Vector3(p.x, p.y, p.z + d)) - m_sdf(Vector3(p.x, p.y, p.z - d))
        );
        return n.normalized();
    }

    // SDF operations
    static SDFunction sphereSDF(const Vector3& center, float radius) {
        return [=](const Vector3& p) {
            return (p - center).length() - radius;
        };
    }

    static SDFunction boxSDF(const Vector3& b) {
        return [=](const Vector3& p) {
            Vector3 q = Vector3(std::abs(p.x), std::abs(p.y), std::abs(p.z)) - b;
            return Vector3(std::max(q.x, 0.0f), std::max(q.y, 0.0f), std::max(q.z, 0.0f)).length() +
                   std::min(std::max(q.x, std::max(q.y, q.z)), 0.0f);
        };
    }

    static SDFunction unionSDF(const SDFunction& sdf1, const SDFunction& sdf2) {
        return [=](const Vector3& p) {
            return std::min(sdf1(p), sdf2(p));
        };
    }

    static SDFunction intersectionSDF(const SDFunction& sdf1, const SDFunction& sdf2) {
        return [=](const Vector3& p) {
            return std::max(sdf1(p), sdf2(p));
        };
    }

    static SDFunction differenceSDF(const SDFunction& sdf1, const SDFunction& sdf2) {
        return [=](const Vector3& p) {
            return std::max(sdf1(p), -sdf2(p));
        };
    }

    static SDFunction blendSDF(const SDFunction& sdf1, const SDFunction& sdf2, float k) {
        return [=](const Vector3& p) {
            float d1 = sdf1(p);
            float d2 = sdf2(p);
            float h = std::clamp(0.5f + 0.5f * (d2 - d1) / k, 0.0f, 1.0f);
            return d1 * (1.0f - h) + d2 * h - k * h * (1.0f - h);
        };
    }

private:
    SDFunction m_sdf;
    float m_epsilon;
};