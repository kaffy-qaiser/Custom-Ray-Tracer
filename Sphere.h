#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include <memory>

class Sphere {
public:
    Vec3 center;
    float radius;
    std::shared_ptr<Material> material;

    Sphere(const Vec3& center, float radius, std::shared_ptr<Material> mat)
            : center(center), radius(radius), material(mat) {}

    bool intersect(const Ray& ray, float t_min, float t_max, float& t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            float temp = (-b - std::sqrt(discriminant)) / (2.0f * a);
            if (temp < t_max && temp > t_min) {
                t = temp;
                return true;
            }
            temp = (-b + std::sqrt(discriminant)) / (2.0f * a);
            if (temp < t_max && temp > t_min) {
                t = temp;
                return true;
            }
        }
        return false;
    }

    Vec3 normal(const Vec3& point) const {
        return (point - center).normalize();
    }
};

#endif // SPHERE_H