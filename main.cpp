#include "pch.hpp"

#include "src/core/Camera.hpp"
#include "src/core/Scene.hpp"
#include "src/objects/Sphere.hpp"
#include "src/objects/Plane.hpp"
#include "src/materials/Phong.hpp"
#include "src/materials/Reflective.hpp"
#include "src/materials/Transparent.hpp"
#include "src/lights/PointLight.hpp"
#include "src/textures/CheckerTexture.hpp"
#include "src/textures/ImageTexture.hpp"

#include <fstream>
#include <iostream>

void writeColor(std::ofstream& out, const Vector3& color) {
    auto clamp = [[(float x) { return std::max(0.0f, std::min(1.0f, x)); };
    out << static_cast<int>(255.999 * clamp(color.x)) << ' '
        << static_cast<int>(255.999 * clamp(color.y)) << ' '
        << static_cast<int>(255.999 * clamp(color.z)) << '\n';
}

// Rest of the code remains the same...

// Function to render the scene and save as PPM image
void render(const Scene& scene, const Camera& camera, int width, int height, const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << "P3\n" << width << ' ' << height << "\n255\n";

    for (int y = 0; y < height; ++y) {
        std::cout << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
        for (int x = 0; x < width; ++x) {
            float u = float(x) / float(width - 1);
            float v = float(y) / float(height - 1);
            Ray ray = camera.generateRay(u, v);

            float t;
            std::shared_ptr<Object> hitObject;
            Vector3 color(0.2, 0.2, 0.2); // Background color

            if (scene.intersect(ray, t, hitObject)) {
                Vector3 hitPoint = ray.pointAtParameter(t);
                Vector3 normal = hitObject->getNormal(hitPoint);
                color = hitObject->getMaterial()->shade(ray, hitPoint, normal, scene);
            }

            writeColor(outFile, color);
        }
    }

    outFile.close();
    std::cout << "\nDone.\n";
}

int main() {
    // Create a scene
    Scene scene;

    // Add objects to the scene
    auto sphere1 = std::make_shared<Sphere>(Vector3(0, 0, -5), 1);
    auto sphere2 = std::make_shared<Sphere>(Vector3(2, 0, -6), 0.5);
    auto plane = std::make_shared<Plane>(Vector3(0, -1, 0), Vector3(0, 1, 0));

    // Create materials
    auto phongMaterial = std::make_shared<Phong>(Vector3(0.7, 0.3, 0.3), Vector3(0.5, 0.5, 0.5), 32);
    auto reflectiveMaterial = std::make_shared<Reflective>(0.8);
    auto transparentMaterial = std::make_shared<Transparent>(1.5);

    // Create a checker texture
    auto checkerTexture = std::make_shared<CheckerTexture>(Vector3(0.2, 0.3, 0.1), Vector3(0.9, 0.9, 0.9), 4);

    // Assign materials to objects
    sphere1->setMaterial(phongMaterial);
    sphere2->setMaterial(reflectiveMaterial);
    plane->setMaterial(std::make_shared<Phong>(checkerTexture, Vector3(0.5, 0.5, 0.5), 16));

    // Add objects to the scene
    scene.addObject(sphere1);
    scene.addObject(sphere2);
    scene.addObject(plane);

    // Add lights to the scene
    auto light1 = std::make_shared<PointLight>(Vector3(5, 5, 0), Vector3(0.6, 0.6, 0.6));
    auto light2 = std::make_shared<PointLight>(Vector3(-5, 3, -3), Vector3(0.4, 0.4, 0.4));
    scene.addLight(light1);
    scene.addLight(light2);

    // Create a camera
    Camera camera(Vector3(0, 1, 2), Vector3(0, 0, -1), Vector3(0, 1, 0), 90, 16.0f/9.0f);

    // Render the scene
    int width = 800;
    int height = 450;
    render(scene, camera, width, height, "output.ppm");

    return 0;
}