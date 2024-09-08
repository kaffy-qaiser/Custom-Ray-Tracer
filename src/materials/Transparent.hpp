#pragma once
#include "Material.hpp"

class Transparent : public Material {
public:
    Transparent(float ior) : m_ior(ior) {}

    Vector3 shade(const Ray& ray, const Vector3& hitPoint, const Vector3& normal,
                  const Scene& scene) const override {
        float cosTheta = Vector3::dot(-ray.direction(), normal);
        float etai = 1.0f, etat = m_ior;
        Vector3 n = normal;
        if (cosTheta < 0) {
            cosTheta = -cosTheta;
            std::swap(etai, etat);
            n = -normal;
        }

        float eta = etai / etat;
        float k = 1.0f - eta * eta * (1.0f - cosTheta * cosTheta);

        if (k < 0) {
            // Total internal reflection
            Vector3 reflectedDir = reflect(ray.direction(), n);
            Ray reflectedRay(hitPoint + n * 0.001f, reflectedDir);
            return traceRay(reflectedRay, scene, ray.depth() + 1);
        } else {
            Vector3 refractedDir = eta * ray.direction() + (eta * cosTheta - std::sqrt(k)) * n;
            Ray refractedRay(hitPoint - n * 0.001f, refractedDir);
            return traceRay(refractedRay, scene, ray.depth() + 1);
        }
    }

private:
    float m_ior; // Index of refraction
};
