#include "utils.h"

#include <chrono>
#include <random>

bool is_nearly_zero(double x, double tolerance) {
    return std::abs(x) < tolerance;
}

bool are_nearly_equal(double x, double y, double tolerance) {
    return std::abs(x - y) < tolerance;
}

bool are_nearly_equal(const Vec3& a, const Vec3& b, double tolerance) {
    return are_nearly_equal(a.x(), b.x(), tolerance) && are_nearly_equal(a.y(), b.y(), tolerance) &&
           are_nearly_equal(a.z(), b.z(), tolerance);
}

bool are_nearly_equal(const Mat4& m, const Mat4& n, double tolerance) {
    const auto& arr1 = m.get_array();
    const auto& arr2 = n.get_array();
    for (size_t i{}; i < 16; ++i) {
        if (!are_nearly_equal(arr1[i], arr2[i])) {
            return false;
        }
    }
    return true;
}

template <>
RgbColor clamp<RgbColor>(const RgbColor& e, const RgbColor& min, const RgbColor& max) {
    auto r = clamp(e.r(), min.r(), max.r());
    auto g = clamp(e.g(), min.g(), max.g());
    auto b = clamp(e.b(), min.b(), max.b());
    return {r, g, b};
}

template <>
Vec3 clamp<Vec3>(const Vec3& e, const Vec3& min, const Vec3& max) {
    auto r = clamp(e.x(), min.x(), max.x());
    auto g = clamp(e.y(), min.y(), max.y());
    auto b = clamp(e.z(), min.z(), max.z());
    return {r, g, b};
}

// NOTE: Vectors provided as arguments are not required to be normalized. The vector returned is not
// guaranteed to be normalized.
Vec3 reflect(const Vec3& v, const Vec3& axis) {
    const auto& n = axis.normalized();
    return 2 * dot(v, n) * n - v;
}

/**
 * @brief Calculate refracted direction or return false if total reflection happens.
 * @param incident  incident direction
 * @param normal    surface normal. The side to which the surface normal points is not required
 * @param eta_a     the refractive index of the side surface normal points to
 * @param eta_b     the refractive index of opposite side
 * @param out_wt    [out] refracted direction
 * @return  true if refraction occurs and false if total reflection occurs
 */
bool refract(const Vec3& incident, const Vec3& normal, double eta_a, double eta_b, Vec3& out_wt) {
    bool same_side = dot(incident, normal) > 0;
    auto wi        = normalized(incident);
    Vec3 n         = normalized(same_side ? normal : -normal);
    double eta_i   = same_side ? eta_a : eta_b;
    double eta_t   = same_side ? eta_b : eta_a;

    auto rel_idx     = eta_i / eta_t;
    auto cos_theta_i = dot(wi, n);
    // auto angle = degree_between(wi, n);
    auto wt_perp = rel_idx * (cos_theta_i * n - wi);

    auto sin_2_theta_i = 1 - cos_theta_i * cos_theta_i;
    auto sin_2_theta_t = rel_idx * rel_idx * sin_2_theta_i;

    if (sin_2_theta_t > 1.0) {
        return false;
    }

    auto cos_theta_t = std::sqrt(1 - sin_2_theta_t);
    auto wt_para     = -cos_theta_t * n;

    out_wt = wt_perp + wt_para;
    return true;
}

double distance(const Vec3& a, const Vec3& b) {
    return (a - b).norm();
}

double degree_between(const Vec3& a, const Vec3& b) {
    return to_degree(std::acos(dot(normalized(a), normalized(b))));
}

CartesianCoordinates::CartesianCoordinates(const SphericalCoordinates& p) : x{}, y{}, z{} {
    using std::cos;
    using std::sin;
    auto [r, theta, phi] = p.components();

    x = r * sin(theta) * cos(phi);
    y = r * cos(theta);
    z = r * sin(theta) * sin(phi);
}

SphericalCoordinates::SphericalCoordinates(const CartesianCoordinates& p) {
    auto [x, y, z] = p.components();
    double xz      = std::sqrt(x * x + z * z);

    theta = std::atan(xz / y);
    phi   = std::atan2(x, z);
    r     = std::sqrt(x * x + y * y + z * z);
}

double absdot(const Vec3& u, const Vec3& v) {
    return std::abs(dot(u, v));
}

double vec_absmax(const Vec3& v) {
    auto [x, y, z] = abs_components(v);
    return std::max({x, y, z});
}

size_t vec_absmax_idx(const Vec3& v) {
    auto [x, y, z] = abs_components(v);
    if (x > y) {
        return x > z ? 0 : 2;
    }
    return y > z ? 1 : 2;
}

RgbColor visualize_normal(const Vec3& normal) {
    auto n = normal.normalized();
    n += Vec3::one();
    n *= 0.5;
    return {n.x(), n.y(), n.z()};
}

const static auto g_Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
static std::mt19937 g_Gen{static_cast<unsigned long>(g_Seed)};

double random_double(double a, double b) {
    std::uniform_real_distribution<double> dist{a, b};
    return dist(g_Gen);
}

int random_int(int a, int b) {
    std::uniform_int_distribution<int> dist{a, b};
    return dist(g_Gen);
}

Vec3 random_vec3(double a, double b) {
    double x{random_double(a, b)};
    double y{random_double(a, b)};
    double z{random_double(a, b)};
    return {x, y, z};
}
