#pragma once
#include "Material.hpp"
#include "../core/Vector3.hpp"
#include <random>

class SubsurfaceScattering : public Material {
public:
    SubsurfaceScattering(const Vector3& albedo, float density)
            : m_albedo(albedo), m_density(density) {}

    bool scatter(const Ray& rayIn, const Vector3& hitPoint, const Vector3& normal,
                 Vector3& attenuation, Vector3& scattered) const override {
        Vector3 scatterDirection = normal + randomUnitVector();
        scattered = Ray(hitPoint, scatterDirection);
        attenuation = m_albedo;

        // Simulate light diffusion inside the material
        float distanceInside = -std::log(getRandom()) / m_density;
        Vector3 newOrigin = hitPoint + scattered.direction() * distanceInside;
        scattered = Ray(newOrigin, randomUnitVector());

        return true;
    }

private:
    Vector3 m_albedo;
    float m_density;

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