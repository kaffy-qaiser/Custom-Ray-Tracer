#ifndef PLANE_H
#define PLANE_H

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include <memory>

class Plane {
public:
    Vec3 point;
    Vec3 normal;
    std::shared_ptr<Material> material;

    Plane(const Vec3& p, const Vec3& n, std::shared_ptr<Material> mat)
            : point(p), normal(n.normalize()), material(mat) {}

    bool intersect(const Ray& ray, float t_min, float t_max, float& t) const {
        float denom = ray.direction.dot(normal);
        if (std::abs(denom) > 1e-6) {
            Vec3 p0l0 = point - ray.origin;
            t = p0l0.dot(normal) / denom;
            return (t >= t_min && t <= t_max);
        }
        return false;
    }
};

#endif // PLANE_H