#pragma once

#include <optional>

#include "intersection.h"
#include "shape.h"
#include "utils.h"

class Light {
  public:
    Light(const TransformedShape& t_shape, const RgbColor& base_color, double intensity);

    virtual ~Light() = default;

    virtual RgbColor compute_emitted_radiance(const Vec3& p, const Vec3& direction) const = 0;

    virtual ShapeSample sample() const = 0;

    virtual std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const;

    TransformedShape get_transformed_shape() const;
    RgbColor get_base_color() const;
    double get_intensity() const;

  private:
    TransformedShape transformed_shape;
    RgbColor base_color{Color::white};
    double intensity{1.0};
};

class SphereLight : public Light {
  public:
};

class AreaLight : public Light {
  public:
    AreaLight(const TransformedShape& t_shape, const RgbColor& base_color, double intensity);

    RgbColor compute_emitted_radiance(const Vec3& p, const Vec3& direction) const override;

    ShapeSample sample() const override;

  private:
};

std::shared_ptr<AreaLight> create_area_light(const Vec3& location,
                                             const Vec3& rotation,
                                             const Vec3& scale,
                                             const RgbColor& base_color = Color::white,
                                             double intensity = 1.0);