#pragma once
#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include <memory>

// Forward declaration
class Material;

class Object {
public:
    virtual ~Object() = default;

    virtual bool intersect(const Ray& ray, float& t) const = 0;
    virtual Vector3 getNormal(const Vector3& point) const = 0;

    void setMaterial(std::shared_ptr<Material> material) {
        m_material = material;
    }

    std::shared_ptr<Material> getMaterial() const {
        return m_material;
    }

    // Optional method for texture coordinate calculation
    virtual void getUV(const Vector3& point, float& u, float& v) const {
        u = 0;
        v = 0;
    }

    // Optional method for bounding box calculation (for acceleration structures)
    virtual void getBoundingBox(Vector3& min, Vector3& max) const {
        min = Vector3(-INFINITY, -INFINITY, -INFINITY);
        max = Vector3(INFINITY, INFINITY, INFINITY);
    }

protected:
    std::shared_ptr<Material> m_material;
};