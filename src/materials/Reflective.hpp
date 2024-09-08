#pragma once
#include "Material.hpp"

class Reflective : public Material {
public:
    Reflective(float reflectivity) : m_reflectivity(reflectivity) {}

    Vector3 shade(const Ray& ray, const Vector3& hitPoint, const Vector3& normal,
                  const Scene& scene) const override {
        Vector3 reflectedDir = reflect(ray.direction(), normal);
        Ray reflectedRay(hitPoint + normal * 0.001f, reflectedDir);

        // Recursively trace the reflected ray (implement a traceRay function in your main renderer)
        Vector3 reflectedColor = traceRay(reflectedRay, scene, ray.depth() + 1);

        return reflectedColor * m_reflectivity;
    }

private:
    float m_reflectivity;
};