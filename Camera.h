// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"
#include <cmath>

class Camera {
public:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect) {
        Vec3 u, v, w;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        origin = lookfrom;
        w = (lookfrom - lookat).normalize();
        u = vup.cross(w).normalize();
        v = w.cross(u);

        lower_left_corner = origin - u * half_width - v * half_height - w;
        horizontal = u * 2 * half_width;
        vertical = v * 2 * half_height;
    }

    Ray get_ray(float s, float t) const {
        Vec3 direction = lower_left_corner + horizontal * s + vertical * t - origin;
        return Ray(origin, direction.normalize());
    }
};

#endif // CAMERA_H