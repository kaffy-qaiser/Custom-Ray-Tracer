#pragma once

#include "../core/Vector3.hpp"
#include "../core/Ray.hpp"
#include "../core/Scene.hpp"
#include "../materials/Material.hpp"
#include "../lights/Light.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <random>

struct Photon {
    Vector3 position;
    Vector3 direction;
    Vector3 power;

    Photon(const Vector3& pos, const Vector3& dir, const Vector3& pow)
            : position(pos), direction(dir), power(pow) {}
};

class PhotonMap {
public:
    PhotonMap() : root(nullptr) {}

    void build(const std::vector<Photon>& photons) {
        nodes = photons;
        root = buildKdTree(0, nodes.size(), 0);
    }

    Vector3 gatherIrradiance(const Vector3& point, float radius, int max_photons) const {
        std::vector<std::pair<float, const Photon*> > nearest_photons;
        gatherPhotons(root, point, radius, max_photons, nearest_photons);

        Vector3 irradiance(0, 0, 0);
        for (const auto& pair : nearest_photons) {
            irradiance = irradiance + pair.second->power;
        }

        float area = M_PI * radius * radius;
        return irradiance / area;
    }

private:
    struct KdTreeNode {
        int index;
        KdTreeNode* left;
        KdTreeNode* right;

        KdTreeNode(int idx) : index(idx), left(nullptr), right(nullptr) {}
    };

    std::vector<Photon> nodes;
    KdTreeNode* root;

    KdTreeNode* buildKdTree(int start, int end, int depth) {
        if (start >= end) return nullptr;

        int mid = (start + end) / 2;
        int axis = depth % 3;

        std::nth_element(nodes.begin() + start, nodes.begin() + mid, nodes.begin() + end,
                         [axis](const Photon& a, const Photon& b) {
                             return a.position.getAxis(axis) < b.position.getAxis(axis);
                         });

        KdTreeNode* node = new KdTreeNode(mid);
        node->left = buildKdTree(start, mid, depth + 1);
        node->right = buildKdTree(mid + 1, end, depth + 1);
        return node;
    }

    void gatherPhotons(const KdTreeNode* node, const Vector3& point, float radius, int max_photons,
                       std::vector<std::pair<float, const Photon*> >& nearest_photons) const {
        if (!node) return;

        const Photon& photon = nodes[node->index];
        float dist_sq = (photon.position - point).lengthSquared();

        if (dist_sq < radius * radius) {
            nearest_photons.emplace_back(dist_sq, &photon);
            std::push_heap(nearest_photons.begin(), nearest_photons.end());

            if (nearest_photons.size() > max_photons) {
                std::pop_heap(nearest_photons.begin(), nearest_photons.end());
                nearest_photons.pop_back();
            }
        }

        int axis = 0; // Determine the split axis based on the node's depth
        float dist_axis = point.getAxis(axis) - photon.position.getAxis(axis);

        if (dist_axis < 0 || (dist_axis * dist_axis < radius * radius && node->right)) {
            gatherPhotons(node->left, point, radius, max_photons, nearest_photons);
        }
        if (dist_axis >= 0 || (dist_axis * dist_axis < radius * radius && node->left)) {
            gatherPhotons(node->right, point, radius, max_photons, nearest_photons);
        }
    }
};

class PhotonTracer {
public:
    PhotonTracer(const Scene& scene, int num_photons)
            : m_scene(scene), m_num_photons(num_photons) {}

    void tracePhotons() {
        std::vector<Photon> photons;
        for (int i = 0; i < m_num_photons; ++i) {
            for (const auto& light : m_scene.getLights()) {
                Ray photon_ray = light->emitPhoton();
                Vector3 photon_power = light->getIntensity() * (1.0f / m_num_photons);
                tracePhoton(photon_ray, photon_power, photons, 0);
            }
        }
        m_photon_map.build(photons);
    }

    Vector3 getIndirectIllumination(const Vector3& point, const Vector3& normal, float radius, int max_photons) const {
        return m_photon_map.gatherIrradiance(point, radius, max_photons);
    }

private:
    const Scene& m_scene;
    int m_num_photons;
    PhotonMap m_photon_map;

    void tracePhoton(const Ray& ray, const Vector3& power, std::vector<Photon>& photons, int depth) {
        if (depth > 5) return; // Max depth to prevent infinite recursion

        float t;
        std::shared_ptr<Object> hit_object;
        if (m_scene.intersect(ray, t, hit_object)) {
            Vector3 hit_point = ray.pointAtParameter(t);
            Vector3 normal = hit_object->getNormal(hit_point);

            // Store photon at the hit point
            photons.emplace_back(hit_point, ray.direction(), power);

            // Russian roulette for photon termination
            if (getRandom() > 0.8f) return;

            // Bounce the photon
            Vector3 new_direction;
            Vector3 attenuation;
            if (hit_object->getMaterial()->scatter(ray, hit_point, normal, attenuation, new_direction)) {
                Vector3 new_power = power * attenuation;
                tracePhoton(Ray(hit_point, new_direction), new_power, photons, depth + 1);
            }
        }
    }

    static float getRandom() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0, 1);
        return dis(gen);
    }
};