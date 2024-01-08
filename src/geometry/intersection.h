#pragma once

#include <memory>
#include <variant>

#include "utils.h"

struct ShadingFrame {
    Vec3 tangent;    // x
    Vec3 normal;     // y
    Vec3 bitangent;  // z
};

// Return (world_to_shading, shading_to_world) transform pair
std::pair<Transform, Transform> shading_transforms(const ShadingFrame& frame);

ShadingFrame generate_world_shading_frame(const Vec3& world_normal);

class Geometry;
class Light;

struct SurfaceIntersection {
    Vec3 p;
    double t;
    ShadingFrame frame;

    /// Inverse direction of incident light
    Vec3 incident;

    /// Pointer to the object with which the ray intersect
    std::variant<std::shared_ptr<Geometry>, std::shared_ptr<Light>> intersection;

    bool is_light() const;

    bool is_geometry() const;

    std::shared_ptr<Geometry> get_geometry() const;

    std::shared_ptr<Light> get_light() const;
};

void transform_shading_frame(const Transform& transform, ShadingFrame& frame);

void transform_hit_record(const Transform& transform, SurfaceIntersection& rec);