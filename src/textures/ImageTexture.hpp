#pragma once
#include "Texture.hpp"
#include <vector>

class ImageTexture : public Texture {
public:
    ImageTexture(const std::string& filename) {
        // Load image file and store in m_data
        // For simplicity, assume we have a function loadImage that does this
        loadImage(filename, m_width, m_height, m_data);
    }

    Vector3 getColor(float u, float v) const override {
        int i = static_cast<int>(u * m_width);
        int j = static_cast<int>(v * m_height);
        i = std::clamp(i, 0, m_width - 1);
        j = std::clamp(j, 0, m_height - 1);
        int index = (j * m_width + i) * 3;
        return Vector3(m_data[index] / 255.0f,
                       m_data[index + 1] / 255.0f,
                       m_data[index + 2] / 255.0f);
    }

private:
    int m_width, m_height;
    std::vector<unsigned char> m_data;
};