#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"

class Material {
public:
    Vec3 albedo;
    float shininess;
    float roughness;

    Material(const Vec3& a, float s, float r) : albedo(a), shininess(s), roughness(r) {}

    virtual bool scatter(const Ray& ray_in, const Vec3& hit_point, const Vec3& normal, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Vec3& a) : Material(a, 0, 1) {}

    virtual bool scatter(const Ray& ray_in, const Vec3& hit_point, const Vec3& normal, Ray& scattered) const override {
        Vec3 target = hit_point + normal + Vec3::random().normalize();
        scattered = Ray(hit_point, target - hit_point);
        return true;
    }
};

class Metal : public Material {
public:
    Metal(const Vec3& a, float r) : Material(a, 1, r) {}

    virtual bool scatter(const Ray& ray_in, const Vec3& hit_point, const Vec3& normal, Ray& scattered) const override {
        Vec3 reflected = ray_in.direction.reflect(normal);
        scattered = Ray(hit_point, reflected + Vec3::random() * roughness);
        return (scattered.direction.dot(normal) > 0);
    }
};

#endif // MATERIAL_H