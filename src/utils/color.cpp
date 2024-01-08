#include "color.h"

#include <cmath>
#include <tuple>

#include "utils.h"

RgbColorU8 to_rgb_u8(const RgbColor& color) {
    auto [r_real, g_real, b_real] = std::make_tuple(color.r(), color.g(), color.b());

    auto r_norm = clamp(r_real, 0.0, 1.0);
    auto g_norm = clamp(g_real, 0.0, 1.0);
    auto b_norm = clamp(b_real, 0.0, 1.0);
    auto r      = static_cast<unsigned char>(r_norm * 254.999);
    auto g      = static_cast<unsigned char>(g_norm * 254.999);
    auto b      = static_cast<unsigned char>(b_norm * 254.999);
    return {r, g, b};
}

RgbColor to_rgb(const RgbColorU8& color) {
    auto clamp = [](double x, double min, double max) {
        return x < min ? min : (x > max ? max : x);
    };
    auto r = clamp(color.r() / 255.0, 0.0, 1.0);
    auto g = clamp(color.g() / 255.0, 0.0, 1.0);
    auto b = clamp(color.b() / 255.0, 0.0, 1.0);
    return {r, g, b};
}

RgbColor& RgbColor::operator+=(const RgbColor& rhs) {
    red += rhs.r();
    green += rhs.g();
    blue += rhs.b();
    return *this;
}

RgbColor& RgbColor::operator-=(const RgbColor& rhs) {
    red -= rhs.r();
    green -= rhs.g();
    blue -= rhs.b();
    return *this;
}

RgbColor& RgbColor::operator*=(const RgbColor& rhs) {
    red *= rhs.r();
    green *= rhs.g();
    blue *= rhs.b();
    return *this;
}

RgbColor& RgbColor::operator/=(const RgbColor& rhs) {
    red /= rhs.r();
    green /= rhs.g();
    blue /= rhs.b();
    return *this;
}

RgbColor operator+(const RgbColor& color1, const RgbColor& color2) {
    auto res = color1;
    return res += color2;
}

RgbColor operator*(double x, const RgbColor& color) {
    RgbColor res = color;
    res *= {x, x, x};
    return res;
}

RgbColor operator*(const RgbColor& color, double x) {
    return x * color;
}

RgbColor operator/(const RgbColor& color, double x) {
    RgbColor res = color;
    res /= {x, x, x};
    return res;
}

bool is_nearly_black(const RgbColor& color, double tolerance) {
    return color.r() < tolerance && color.g() < tolerance && color.b() < tolerance;
}

RgbColor operator*(const RgbColor& color1, const RgbColor& color2) {
    RgbColor res = color1;
    res *= {color2.r(), color2.g(), color2.b()};
    return res;
}

std::ostream& operator<<(std::ostream& os, const RgbColor& color) {
    os << std::fixed << std::setprecision(3);
    os << "(" << std::setw(7) << color.r() << ", ";
    os << std::setw(7) << color.g() << ", ";
    os << std::setw(7) << color.b() << ")";
    return os;
}
