#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <memory>
#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"

class RayTracer {
public:
    RayTracer(int width, int height);
    void render();

private:
    int width, height;
    std::vector<std::shared_ptr<Sphere>> spheres;
    std::vector<std::shared_ptr<Plane>> planes;
    std::vector<Light> lights;
    Camera camera;

    Vec3 color(const Ray& ray, int depth);
    bool hit_object(const Ray& ray, float t_min, float t_max, float& t, Vec3& normal, std::shared_ptr<Material>& material);
    bool is_shadowed(const Vec3& hit_point, const Vec3& light_dir, float light_distance);
    Vec3 random_in_unit_sphere();
};

#endif // RAYTRACER_H