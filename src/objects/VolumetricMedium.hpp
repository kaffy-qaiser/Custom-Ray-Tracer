#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "Object.hpp"
#include <memory>
#include <cmath>
#include <random>

class VolumetricMedium : public Object {
public:
    VolumetricMedium(std::shared_ptr<Object> boundary, float density, const Vector3& albedo)
            : m_boundary(boundary), m_density(density), m_albedo(albedo) {}

    bool intersect(const Ray& ray, float& t) const override {
        float t0, t1;
        if (!m_boundary->intersect(ray, t0) || !m_boundary->intersect(Ray(ray.pointAtParameter(t0 + 0.0001f), ray.direction()), t1)) {
            return false;
        }

        t0 = std::max(t0, 0.0f);
        t1 = std::min(t1, std::numeric_limits<float>::max());

        if (t0 >= t1) return false;

        float distanceInsideBoundary = (t1 - t0) * ray.direction().length();
        float hitDistance = -std::log(getRandom()) / m_density;

        if (hitDistance > distanceInsideBoundary) {
            return false;
        }

        t = t0 + hitDistance / ray.direction().length();
        return true;
    }

    Vector3 getNormal(const Vector3& point) const override {
        return Vector3(1, 0, 0);  // Arbitrary for volumetric medium
    }

    bool scatter(const Ray& rayIn, const Vector3& hitPoint, const Vector3& normal,
                 Vector3& attenuation, Ray& scattered) const {
        scattered = Ray(hitPoint, randomUnitVector());
        attenuation = m_albedo;
        return true;
    }

private:
    std::shared_ptr<Object> m_boundary;
    float m_density;
    Vector3 m_albedo;

    static Vector3 randomUnitVector() {
        static std::mt19937 gen(std::random_device{}());
        static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        while (true) {
            Vector3 v(dist(gen), dist(gen), dist(gen));
            if (v.length() < 1) return v.normalized();
        }
    }

    static float getRandom() {
        static std::mt19937 gen(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(gen);
    }
};