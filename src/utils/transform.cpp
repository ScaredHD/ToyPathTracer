#include "transform.h"

#include "ray.h"
#include "utils.h"

Transform::Transform(const Vec3& t, const Vec3& r, const Vec3& s)
    : mat{translate(t) * rotate(r) * scale(s)},
      inv_mat{mat.inverse()} {
}

Transform::Transform(const Mat4& mat) : mat{mat}, inv_mat{mat.inverse()} {
}

Transform Transform::operator*(const Transform& rhs) const {
    return {mat * rhs.get_mat()};
}

Vec3 Transform::on_point(const Vec3& p) const {
    return hnormalized_point(mat * homogeneous_point(p));
}

Vec3 Transform::on_vec(const Vec3& v) const {
    return hnormalized_vec(mat * homogeneous_vec(v));
}

Vec3 Transform::on_normal(const Vec3& n) const {
    return hnormalized_vec(inv_mat.transposed() * homogeneous_vec(n));
}

Ray Transform::on_ray(const Ray& r) const {
    return {on_point(r.o), on_vec(r.d)};
}

void Transform::apply_on_point(Vec3& p) const {
    p = on_point(p);
}

void Transform::apply_on_vec(Vec3& v) const {
    v = on_vec(v);
}

void Transform::apply_on_normal(Vec3& n) const {
    n = on_normal(n);
}

void Transform::apply_on_light(Ray& r) const {
    r = on_ray(r);
}

Vec3 Transform::extract_sacle() const {
    auto c0 = Vec3{mat(0, 0), mat(1, 0), mat(2, 0)};
    auto c1 = Vec3{mat(0, 1), mat(1, 1), mat(2, 1)};
    auto c2 = Vec3{mat(0, 2), mat(1, 2), mat(2, 2)};
    auto sx = c0.norm();
    auto sy = c1.norm();
    auto sz = c2.norm();
    return {sx, sy, sz};
}

bool Transform::is_uniform_scale() const {
    auto [sx, sy, sz] = components(extract_sacle());
    return (sx == sy) && (sy == sz);
}

double Transform::uniform_scaling_factor() const {
    return is_uniform_scale() ? extract_sacle().x() : 0.0;
}

Mat4 translate(const Vec3& displacement) {
    auto [dx, dy, dz] = components(displacement);
    // clang-format off
    return {
        1, 0, 0, dx,
        0, 1, 0, dy,
        0, 0, 1, dz,
        0, 0, 0,  1
    };
    // clang-format on
}

Mat4 rotate_x(double deg) {
    auto [cost, sint] = std::make_tuple(std::cos(to_radian(deg)), std::sin(to_radian(deg)));
    // clang-format off
    return {
        1,    0,     0, 0,
        0, cost, -sint, 0,
        0, sint,  cost, 0,
        0,    0,     0, 1
    };
    // clang-format on
}

Mat4 rotate_y(double deg) {
    auto [cost, sint] = std::make_tuple(std::cos(to_radian(deg)), std::sin(to_radian(deg)));
    // clang-format off
    return {
         cost, 0, sint, 0,
            0, 1,    0, 0,
        -sint, 0, cost, 0,
            0, 0,    0, 1
    };
    // clang-format on
}

Mat4 rotate_z(double deg) {
    auto [cost, sint] = std::make_tuple(std::cos(to_radian(deg)), std::sin(to_radian(deg)));
    // clang-format off
    return {
        cost, -sint, 0, 0,
        sint,  cost, 0, 0,
           0,     0, 1, 0,
           0,     0, 0, 1
    };
    // clang-format on
}

Mat4 rotate(const Vec3& degrees) {
    auto [dx, dy, dz] = components(degrees);
    return rotate_z(dz) * rotate_y(dy) * rotate_x(dx);
}

Mat4 scale(const Vec3& scales) {
    auto [sx, sy, sz] = components(scales);
    // clang-format off
    return {
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    };
    // clang-format on
}
