#pragma once

#include "vec.h"

class Ray {
  public:
    Ray(const Vec3& o, const Vec3& d) : o{o}, d{d} {}

    Vec3 at(double t) const { return o + t * d; }

    Vec3 o;
    Vec3 d;
};