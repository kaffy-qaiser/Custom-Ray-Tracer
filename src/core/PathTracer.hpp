#pragma once
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vector3.hpp"
#include "ImportanceSampler.hpp"
#include "../objects/VolumetricMedium.hpp"
#include "../materials/Material.hpp"
#include <random>
#include <memory>
#include <algorithm>

class PathTracer {
public:
    PathTracer(int maxDepth = 5) : m_maxDepth(maxDepth) {}

    Vector3 trace(const Ray& ray, const Scene& scene, int depth = 0) {
        if (depth >= m_maxDepth) {
            return Vector3(0, 0, 0); // Black if max depth reached
        }

        float t;
        std::shared_ptr<Object> hitObject;
        if (!scene.intersect(ray, t, hitObject)) {
            return Vector3(0.2, 0.2, 0.2); // Background color
        }

        Vector3 hitPoint = ray.pointAtParameter(t);
        Vector3 normal = hitObject->getNormal(hitPoint);
        Vector3 color = hitObject->getMaterial()->emitted();

        // Handle volumetric media
        if (auto* medium = dynamic_cast<VolumetricMedium*>(hitObject.get())) {
            Vector3 scattered = ImportanceSampler::sampleHemisphere(normal);
            Ray scatteredRay(hitPoint, scattered);
            float pdf = ImportanceSampler::pdfHemisphere(normal, scattered);

            Vector3 attenuation;
            if (medium->scatter(ray, hitPoint, normal, attenuation, scatteredRay)) {
                return attenuation * trace(scatteredRay, scene, depth + 1) / pdf;
            }
            return Vector3(0, 0, 0);
        }

        // Russian Roulette for path termination
        float p = std::max(std::max(color.x, color.y), std::max(color.z, 0.01f));
        if (getRandom() > p) {
            return color;
        }
        color = color * (1.0f / p);

        Vector3 scattered = ImportanceSampler::sampleHemisphere(normal);
        float pdf = ImportanceSampler::pdfHemisphere(normal, scattered);

        Vector3 attenuation;
        Ray scatteredRay(hitPoint, scattered);
        if (hitObject->getMaterial()->scatter(ray, hitPoint, normal, attenuation, scatteredRay)) {
            color = color + attenuation * trace(scatteredRay, scene, depth + 1) / pdf;
        }

        return color;
    }

private:
    int m_maxDepth;

    static float getRandom() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(gen);
    }
};