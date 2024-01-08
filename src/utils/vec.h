#pragma once

// TODO: Test VectorX

#include <tuple>

#include "mat.h"

template <size_t len>
using VecX = MatX<len, 1>;

using Vec2 = VecX<2>;
using Vec3 = VecX<3>;
using Vec4 = VecX<4>;

inline std::tuple<double, double> components(const Vec2& v) {
    return {v.x(), v.y()};
}

inline std::tuple<double, double, double> components(const Vec3& v) {
    return {v.x(), v.y(), v.z()};
}

inline std::tuple<double, double, double, double> components(const Vec4& v) {
    return {v.x(), v.y(), v.z(), v.w()};
}

inline std::tuple<double, double> abs_components(const Vec2& v) {
    return {std::abs(v.x()), std::abs(v.y())};
}

inline std::tuple<double, double, double> abs_components(const Vec3& v) {
    return {std::abs(v.x()), std::abs(v.y()), std::abs(v.z())};
}

inline std::tuple<double, double, double, double> abs_components(const Vec4& v) {
    return {std::abs(v.x()), std::abs(v.y()), std::abs(v.z()), std::abs(v.w())};
}

template <size_t len>
double dot(const VecX<len>& u, const VecX<len>& v) {
    return u.transposed() * v;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    const auto& [ux, uy, uz] = components(u);
    const auto& [vx, vy, vz] = components(v);
    auto x                   = uy * vz - uz * vy;
    auto y                   = uz * vx - ux * vz;
    auto z                   = ux * vy - uy * vx;
    return {x, y, z};
}

template <size_t len>
double norm(const VecX<len>& v) {
    return v.norm();
}

template <size_t len>
VecX<len> normalized(const VecX<len>& v) {
    return v.normalized();
}

inline Vec4 homogeneous(const Vec3& v, double x) {
    return {v.x(), v.y(), v.z(), x};
}

inline Vec4 homogeneous_point(const Vec3& v) {
    return homogeneous(v, 1.0);
}

inline Vec3 hnormalized_point(const Vec4& v) {
    return Vec3{v.x(), v.y(), v.z()} / v.w();
}

inline Vec4 homogeneous_vec(const Vec3& v) {
    return homogeneous(v, 0.0);
}

inline Vec3 hnormalized_vec(const Vec4& v) {
    return Vec3{v.x(), v.y(), v.z()};
}

template <size_t len>
std::ostream& operator<<(std::ostream& os, const VecX<len>& v) {
    os << '(';
    for (size_t i{}; i < len - 1; ++i) {
        os << std::fixed << std::setprecision(3);
        os << std::setw(7) << v[i] << ", ";
    }
    os << std::fixed << std::setprecision(3);
    os << std::setw(7) << v[len - 1] << ")";
    return os;
}
