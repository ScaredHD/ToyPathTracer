#pragma once

#include <ostream>

class RgbColor {
  public:
    RgbColor() : RgbColor(0, 0, 0) {}

    RgbColor(double r, double g, double b) : red{r}, green{g}, blue{b} {}

    double& r() { return red; }

    double& g() { return green; }

    double& b() { return blue; }

    const double& r() const { return red; }

    const double& g() const { return green; }

    const double& b() const { return blue; }

    RgbColor& operator+=(const RgbColor& rhs);
    RgbColor& operator-=(const RgbColor& rhs);
    RgbColor& operator*=(const RgbColor& rhs);
    RgbColor& operator/=(const RgbColor& rhs);

  private:
    double red;
    double green;
    double blue;
};

class RgbColorU8 {
  public:
    RgbColorU8(unsigned char r, unsigned char g, unsigned char b) : red{r}, green{g}, blue{b} {}

    unsigned char& r() { return red; }

    unsigned char& g() { return green; }

    unsigned char& b() { return blue; }

    const unsigned char& r() const { return red; }

    const unsigned char& g() const { return green; }

    const unsigned char& b() const { return blue; }

  private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

// Predefined colors
namespace Color {
// Primary colors
const RgbColor red{1.0, 0.0, 0.0};
const RgbColor green{0.0, 1.0, 0.0};
const RgbColor blue{0.0, 0.0, 1.0};

// Secondary colors
const RgbColor yellow{1.0, 1.0, 0.0};
const RgbColor cyan{0.0, 1.0, 1.0};
const RgbColor magenta{1.0, 0.0, 1.0};

// Useful colors
const RgbColor black{0.0, 0.0, 0.0};
const RgbColor white{1.0, 1.0, 1.0};

const RgbColor skyblue{0.5, 0.7, 1.0};

}  // namespace Color

RgbColor to_rgb(const RgbColorU8& color);
RgbColorU8 to_rgb_u8(const RgbColor& color);

RgbColor operator+(const RgbColor& color1, const RgbColor& color2);
RgbColor operator*(const RgbColor& color1, const RgbColor& color2);
RgbColor operator*(double x, const RgbColor& color);
RgbColor operator*(const RgbColor& color, double x);
RgbColor operator/(const RgbColor& color, double x);

bool is_nearly_black(const RgbColor& color, double tolerance = 1e-6);

std::ostream& operator<<(std::ostream& os, const RgbColor& color);
