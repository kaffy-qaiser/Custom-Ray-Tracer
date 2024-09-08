#pragma once
#include "Vector3.hpp"
#include <random>
#include <cmath>

class ImportanceSampler {
public:
    static Vector3 sampleHemisphere(const Vector3& normal) {
        float u1 = getRandom();
        float u2 = getRandom();
        float r = std::sqrt(1.0f - u1 * u1);
        float phi = 2 * M_PI * u2;
        Vector3 tangent, bitangent;
        createCoordinateSystem(normal, tangent, bitangent);
        return (tangent * std::cos(phi) * r + bitangent * std::sin(phi) * r + normal * u1).normalized();
    }

    static float pdfHemisphere(const Vector3& normal, const Vector3& direction) {
        float cosTheta = Vector3::dot(normal, direction);
        return cosTheta > 0 ? cosTheta / M_PI : 0;
    }

private:
    static void createCoordinateSystem(const Vector3& normal, Vector3& tangent, Vector3& bitangent) {
        if (std::abs(normal.x) > std::abs(normal.y))
            tangent = Vector3(normal.z, 0, -normal.x).normalized();
        else
            tangent = Vector3(0, -normal.z, normal.y).normalized();
        bitangent = Vector3::cross(normal, tangent);
    }

    static float getRandom() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(gen);
    }
};