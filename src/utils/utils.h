#pragma once

#include "color.h"
#include "logger.h"
#include "mat.h"
#include "transform.h"
#include "vec.h"

constexpr double inf{std::numeric_limits<double>::infinity()};
constexpr double pi{3.14159265359};
constexpr double pi_div_2{1.57079632679};
constexpr double pi_div_4{0.78539816339};
constexpr double inv_pi{0.31830988618};
constexpr double inv_2pi{0.15915494309};
constexpr double inv_4pi{0.07957747154};

bool is_nearly_zero(double x, double tolerance = 1.e-7);

bool are_nearly_equal(double x, double y, double tolerance = 1.e-7);

bool are_nearly_equal(const Vec3& a, const Vec3& b, double tolerance = 1.e-7);

bool are_nearly_equal(const Mat4& m, const Mat4& n, double tolerance = 1.e-7);

inline double to_radian(double deg) {
    return deg * pi / 180.0;
}

inline double to_degree(double rad) {
    return rad * 180.0 / pi;
}

template <typename T>
T lerp(const T& a, const T& b, double t) {
    return (1 - t) * a + t * b;
}

template <typename T>
T clamp(const T& e, const T& min, const T& max) {
    return e < min ? min : (e > max ? max : e);
}

template <>
RgbColor clamp<RgbColor>(const RgbColor& e, const RgbColor& min, const RgbColor& max);

template <>
Vec3 clamp<Vec3>(const Vec3& e, const Vec3& min, const Vec3& max);

Vec3 reflect(const Vec3& v, const Vec3& axis);

bool refract(const Vec3& incident, const Vec3& normal, double eta_a, double eta_b, Vec3& out_wt);

double distance(const Vec3& a, const Vec3& b);

double degree_between(const Vec3& a, const Vec3& b);

class SphericalCoordinates;

// Right handed, y up
class CartesianCoordinates {
  public:
    CartesianCoordinates(double x, double y, double z) : x{x}, y{y}, z{z} {}

    CartesianCoordinates(const SphericalCoordinates& p);

    Vec3 to_vec() const { return {x, y, z}; }

    std::tuple<double, double, double> components() const { return {x, y, z}; }

  private:
    double x;
    double y;
    double z;
};

// Phi: azimuth angle
// Theta: polar angle
class SphericalCoordinates {
  public:
    SphericalCoordinates(double r, double theta, double phi) : r{r}, theta{theta}, phi{phi} {}

    SphericalCoordinates(const CartesianCoordinates& p);

    std::tuple<double, double, double> components() const { return {r, theta, phi}; }

  private:
    double r;
    double theta;
    double phi;
};

double absdot(const Vec3& u, const Vec3& v);

double vec_absmax(const Vec3& v);

size_t vec_absmax_idx(const Vec3& v);

RgbColor visualize_normal(const Vec3& normal);

double random_double(double a = 0.0, double b = 1.0);

int random_int(int a = 0, int b = 1);

Vec3 random_vec3(double a, double b);
