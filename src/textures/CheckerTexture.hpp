#pragma once
#include "../core/Vector3.hpp"
#include "Texture.hpp"
#include <cmath>

class CheckerTexture : public Texture {
public:
    CheckerTexture(const Vector3& color1, const Vector3& color2, float scale)
            : m_color1(color1), m_color2(color2), m_scale(scale) {}

    Vector3 getColor(float u, float v) const override {
        float sines = std::sin(m_scale * u * M_PI) * std::sin(m_scale * v * M_PI);
        return sines < 0 ? m_color1 : m_color2;
    }

private:
    Vector3 m_color1;
    Vector3 m_color2;
    float m_scale;
};