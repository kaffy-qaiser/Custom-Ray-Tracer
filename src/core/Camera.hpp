#pragma once
#include "Ray.hpp"
#include "Matrix4.hpp"

class Camera {
public:
    Camera(const Vector3& position, const Vector3& lookAt, const Vector3& up,
           float fov, float aspectRatio)
            : m_position(position), m_fov(fov), m_aspectRatio(aspectRatio) {
        Vector3 w = (position - lookAt).normalized();
        Vector3 u = Vector3::cross(up, w).normalized();
        Vector3 v = Vector3::cross(w, u);

        m_viewMatrix = Matrix4(u.x, u.y, u.z, -Vector3::dot(u, position),
                               v.x, v.y, v.z, -Vector3::dot(v, position),
                               w.x, w.y, w.z, -Vector3::dot(w, position),
                               0,   0,   0,   1);
    }

    Ray generateRay(float x, float y) const {
        float tanHalfFov = std::tan(m_fov * 0.5f * M_PI / 180.0f);
        float screenX = (2.0f * x - 1.0f) * tanHalfFov * m_aspectRatio;
        float screenY = (1.0f - 2.0f * y) * tanHalfFov;

        Vector3 rayDirection = Vector3(screenX, screenY, -1.0f).normalized();
        rayDirection = m_viewMatrix.inverse().transformVector(rayDirection);

        return Ray(m_position, rayDirection);
    }

private:
    Vector3 m_position;
    float m_fov;
    float m_aspectRatio;
    Matrix4 m_viewMatrix;
};
