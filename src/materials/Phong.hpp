#pragma once
#include "Material.hpp"

class Phong : public Material {
public:
    Phong(const Vector3& diffuse, const Vector3& specular, float shininess)
            : m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}

    Vector3 shade(const Ray& ray, const Vector3& hitPoint, const Vector3& normal,
                  const Scene& scene) const override {
        Vector3 color(0, 0, 0);

        for (const auto& light : scene.getLights()) {
            Vector3 lightDir = (light->getPosition() - hitPoint).normalized();
            Vector3 viewDir = -ray.direction();

            // Diffuse
            float diff = std::max(Vector3::dot(normal, lightDir), 0.0f);
            Vector3 diffuse = m_diffuse * diff;

            // Specular
            Vector3 reflectDir = reflect(-lightDir, normal);
            float spec = std::pow(std::max(Vector3::dot(viewDir, reflectDir), 0.0f), m_shininess);
            Vector3 specular = m_specular * spec;

            color += light->getIntensity() * (diffuse + specular);
        }

        return color;
    }

private:
    Vector3 m_diffuse;
    Vector3 m_specular;
    float m_shininess;
};