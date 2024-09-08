#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "../objects/Object.hpp"

class Material {
public:
    virtual Vector3 shade(const Ray& ray, const Vector3& hitPoint, const Vector3& normal,
                          const Scene& scene) const = 0;
    virtual ~Material() = default;

    virtual Vector3 emitted() const {
        return Vector3(0, 0, 0);
    }

    bool scatter(const Ray& ray, const Vector3& hitPoint, const Vector3& normal, Vector3& attenuation, Ray& scatteredRay);
};