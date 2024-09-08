#pragma once
#include "Vector3.hpp"
#include <array>
#include <cmath>

class Matrix4 {
public:
    Matrix4() {
        m_data.fill(0.0f);
        m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1.0f;
    }

    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) {
        m_data = {m00, m01, m02, m03,
                  m10, m11, m12, m13,
                  m20, m21, m22, m23,
                  m30, m31, m32, m33};
    }

    float& operator()(int row, int col) {
        return m_data[row * 4 + col];
    }

    const float& operator()(int row, int col) const {
        return m_data[row * 4 + col];
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = 0;
                for (int k = 0; k < 4; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    Vector3 transformPoint(const Vector3& v) const {
        float w = m_data[3] * v.x + m_data[7] * v.y + m_data[11] * v.z + m_data[15];
        return Vector3(
                (m_data[0] * v.x + m_data[4] * v.y + m_data[8] * v.z + m_data[12]) / w,
                (m_data[1] * v.x + m_data[5] * v.y + m_data[9] * v.z + m_data[13]) / w,
                (m_data[2] * v.x + m_data[6] * v.y + m_data[10] * v.z + m_data[14]) / w
        );
    }

    Vector3 transformVector(const Vector3& v) const {
        return Vector3(
                m_data[0] * v.x + m_data[4] * v.y + m_data[8] * v.z,
                m_data[1] * v.x + m_data[5] * v.y + m_data[9] * v.z,
                m_data[2] * v.x + m_data[6] * v.y + m_data[10] * v.z
        );
    }

    Matrix4 inverse() const {
        // Implement matrix inversion (e.g., using Gauss-Jordan elimination)
        return Matrix4();
    }

    static Matrix4 translation(const Vector3& t) {
        return Matrix4(
                1, 0, 0, t.x,
                0, 1, 0, t.y,
                0, 0, 1, t.z,
                0, 0, 0, 1
        );
    }

    static Matrix4 scaling(const Vector3& s) {
        return Matrix4(
                s.x, 0, 0, 0,
                0, s.y, 0, 0,
                0, 0, s.z, 0,
                0, 0, 0, 1
        );
    }

    static Matrix4 rotationX(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4(
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1
        );
    }

    static Matrix4 rotationY(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4(
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
        );
    }

    static Matrix4 rotationZ(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Matrix4(
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        );
    }

private:
    std::array<float, 16> m_data;
};