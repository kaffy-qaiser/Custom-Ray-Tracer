#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include <random>

class Light {
public:
    virtual ~Light() = default;

    virtual Vector3 getPosition() const = 0;
    virtual Vector3 getIntensity() const = 0;

    // Optional method for area lights
    virtual Vector3 getRandomPoint() const {
        return getPosition();
    }

    // Optional method for directional lights
    virtual Vector3 getDirection() const {
        return Vector3(0, 0, 0);
    }

    // Optional method for attenuation
    virtual float getAttenuation(float distance) const {
        return 1.0f;
    }

    // New method for photon emission
    virtual Ray emitPhoton() const {
        Vector3 origin = getRandomPoint();
        Vector3 direction = getRandomDirection();
        return Ray(origin, direction);
    }

protected:
    // Helper method to generate random directions
    Vector3 getRandomDirection() const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(-1.0, 1.0);

        Vector3 direction;
        do {
            direction = Vector3(dis(gen), dis(gen), dis(gen));
        } while (direction.lengthSquared() > 1.0);

        return direction.normalized();
    }
};