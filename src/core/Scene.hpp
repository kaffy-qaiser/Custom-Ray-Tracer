#pragma once
#include <vector>
#include "../objects/Object.hpp"
#include "../lights/Light.hpp"

class Scene {
public:
    void addObject(std::shared_ptr<Object> object) {
        m_objects.push_back(object);
    }

    void addLight(std::shared_ptr<Light> light) {
        m_lights.push_back(light);
    }

    bool intersect(const Ray& ray, float& t, std::shared_ptr<Object>& hitObject) const {
        float closestT = std::numeric_limits<float>::max();
        std::shared_ptr<Object> closestObject = nullptr;

        for (const auto& object : m_objects) {
            float currentT;
            if (object->intersect(ray, currentT) && currentT < closestT) {
                closestT = currentT;
                closestObject = object;
            }
        }

        if (closestObject) {
            t = closestT;
            hitObject = closestObject;
            return true;
        }
        return false;
    }

    const std::vector<std::shared_ptr<Light>>& getLights() const { return m_lights; }

private:
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;
};