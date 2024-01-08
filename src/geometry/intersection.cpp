#include "intersection.h"

bool SurfaceIntersection::is_light() const {
    return std::holds_alternative<std::shared_ptr<Light>>(intersection);
}

bool SurfaceIntersection::is_geometry() const {
    return std::holds_alternative<std::shared_ptr<Geometry>>(intersection);
}

std::shared_ptr<Geometry> SurfaceIntersection::get_geometry() const {
    return std::get<std::shared_ptr<Geometry>>(intersection);
}

std::shared_ptr<Light> SurfaceIntersection::get_light() const {
    return std::get<std::shared_ptr<Light>>(intersection);
}

std::pair<Transform, Transform> shading_transforms(const ShadingFrame& frame) {
    auto [tx, ty, tz] = components(frame.tangent);
    auto [nx, ny, nz] = components(frame.normal);
    auto [bx, by, bz] = components(frame.bitangent);

    // clang-format off
    Mat4 wts {
        {tx, ty, tz, 0},
        {nx, ny, nz, 0}, 
        {bx, by, bz, 0}, 
        { 0,  0,  0, 1}
    };
    Mat4 stw {
        {tx, nx, bx, 0}, 
        {ty, ny, by, 0}, 
        {tz, nz, bz, 0},
        { 0,  0,  0, 1}
    };
    // clang-format on

    Transform world_to_shading = wts;
    Transform shading_to_world = stw;

    return {world_to_shading, shading_to_world};
}

ShadingFrame generate_world_shading_frame(const Vec3& world_normal) {
    auto [x, y, z] = components(world_normal);

    // ---------- Shuffle normal ----------
    auto idx{vec_absmax_idx(world_normal)};
    Vec3 v;
    v[0] = (idx == 1 ? -y : y);
    v[1] = (idx == 2 ? -z : z);
    v[2] = (idx == 0 ? -x : x);

    auto n = world_normal.normalized();
    auto t = cross(n, v).normalized();
    auto b = cross(t, n);
    return {t, n, b};
}

void transform_shading_frame(const Transform& transform, ShadingFrame& frame) {
    transform.apply_on_normal(frame.normal);
    transform.apply_on_vec(frame.tangent);
    transform.apply_on_vec(frame.bitangent);
}

void transform_hit_record(const Transform& transform, SurfaceIntersection& rec) {
    transform.apply_on_point(rec.p);
    transform.apply_on_vec(rec.incident);
    transform_shading_frame(transform, rec.frame);
}
