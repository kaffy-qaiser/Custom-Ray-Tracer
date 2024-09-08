#pragma once
#include "../core/Vector3.hpp"
#include "Light.hpp"

class PointLight : public Light {
public:
    PointLight(const Vector3& position, const Vector3& intensity)
            : m_position(position), m_intensity(intensity) {}

    Vector3 getPosition() const override {
        return m_position;
    }

    Vector3 getIntensity() const override {
        return m_intensity;
    }

    // Optional: Implement attenuation
    float getAttenuation(float distance) const override {
        return 1.0f / (distance * distance);
    }

private:
    Vector3 m_position;
    Vector3 m_intensity;
};