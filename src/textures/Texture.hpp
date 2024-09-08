#pragma once
#include "../core/Vector3.hpp"

class Texture {
public:
    virtual ~Texture() = default;

    virtual Vector3 getColor(float u, float v) const = 0;

    // Optional method for bump mapping
    virtual Vector3 getNormal(float u, float v, const Vector3& geometryNormal) const {
        return geometryNormal;
    }

    // Optional method for displacement mapping
    virtual float getDisplacement(float u, float v) const {
        return 0.0f;
    }
};