#include "RayTracer.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <random>

RayTracer::RayTracer(int width, int height)
        : width(width), height(height),
          camera(Vec3(0, 0, -1), Vec3(0, 0, 0), Vec3(0, 1, 0), 90, float(width) / float(height)) {
    // Add spheres
    spheres.push_back(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Vec3(0.8, 0.3, 0.3))));
    spheres.push_back(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0))));
    spheres.push_back(std::make_shared<Sphere>(Vec3(1, 0, -1), 0.5, std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.3)));
    spheres.push_back(std::make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 0.0)));

    // Add planes
    planes.push_back(std::make_shared<Plane>(Vec3(0, -0.5, 0), Vec3(0, 1, 0), std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

    // Add lights
    lights.push_back(Light(Vec3(2, 2, -1), Vec3(1, 1, 1), 1.0));
    lights.push_back(Light(Vec3(-2, 2, -1), Vec3(0.5, 0.5, 1), 0.5));
    lights.push_back(Light(Vec3(0, -2, -1), Vec3(1, 0.5, 0.5), -0.3)); // Negative light
}

void RayTracer::render() {
    std::ofstream out("output.ppm");
    out << "P3\n" << width << " " << height << "\n255\n";

    int samples_per_pixel = 100;

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            Vec3 col(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                float u = float(i + drand48()) / float(width);
                float v = float(j + drand48()) / float(height);
                Ray r = camera.get_ray(u, v);
                col = col + color(r, 0);
            }
            col = col / float(samples_per_pixel);
            col = Vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z)); // Gamma correction

            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);

            out << ir << " " << ig << " " << ib << "\n";
        }
    }
}

Vec3 RayTracer::color(const Ray& r, int depth) {
    float t;
    Vec3 normal;
    std::shared_ptr<Material> material;

    if (hit_object(r, 0.001, std::numeric_limits<float>::max(), t, normal, material)) {
        Ray scattered;
        Vec3 attenuation;
        Vec3 hit_point = r.point_at_parameter(t);

        if (depth < 50 && material->scatter(r, hit_point, normal, scattered)) {
            Vec3 light_contribution(0, 0, 0);

            for (const auto& light : lights) {
                Vec3 light_dir = (light.position - hit_point).normalize();
                float light_distance = (light.position - hit_point).magnitude();

                if (!is_shadowed(hit_point, light_dir, light_distance)) {
                    float diffuse = std::max(0.0f, normal.dot(light_dir));
                    Vec3 diffuse_color = material->albedo * light.color * light.intensity * diffuse;

                    Vec3 view_dir = (camera.origin - hit_point).normalize();
                    Vec3 half_dir = (view_dir + light_dir).normalize();
                    float specular = std::pow(std::max(0.0f, normal.dot(half_dir)), material->shininess * 128.0f);
                    Vec3 specular_color = light.color * light.intensity * specular;

                    light_contribution = light_contribution + diffuse_color + specular_color;
                }
            }

            return light_contribution * color(scattered, depth + 1);
        } else {
            return Vec3(0, 0, 0);
        }
    } else {
        Vec3 unit_direction = r.direction.normalize();
        float t = 0.5 * (unit_direction.y + 1.0);
        return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.5, 0.7, 1.0) * t;
    }
}

bool RayTracer::hit_object(const Ray& ray, float t_min, float t_max, float& t, Vec3& normal, std::shared_ptr<Material>& material) {
    float closest_so_far = t_max;
    bool hit_anything = false;

    for (const auto& sphere : spheres) {
        if (sphere->intersect(ray, t_min, closest_so_far, t)) {
            hit_anything = true;
            closest_so_far = t;
            normal = sphere->normal(ray.point_at_parameter(t));
            material = sphere->material;
        }
    }

    for (const auto& plane : planes) {
        if (plane->intersect(ray, t_min, closest_so_far, t)) {
            hit_anything = true;
            closest_so_far = t;
            normal = plane->normal;
            material = plane->material;
        }
    }

    return hit_anything;
}

bool RayTracer::is_shadowed(const Vec3& hit_point, const Vec3& light_dir, float light_distance) {
    Ray shadow_ray(hit_point + light_dir * 0.001, light_dir);
    float t;
    Vec3 normal;
    std::shared_ptr<Material> material;

    return hit_object(shadow_ray, 0.001, light_distance, t, normal, material);
}

Vec3 RayTracer::random_in_unit_sphere() {
    Vec3 p;
    do {
        p = Vec3::random(-1, 1);
    } while (p.dot(p) >= 1.0);
    return p;
}

