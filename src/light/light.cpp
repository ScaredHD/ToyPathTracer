#include "light.h"

Light::Light(const TransformedShape& t_shape, const RgbColor& base_color, double intensity)
    : transformed_shape{t_shape},
      base_color{base_color},
      intensity{intensity} {}

std::optional<SurfaceIntersection> Light::hit(const Ray& ray, double tmin, double tmax) const {
    return transformed_shape.hit(ray, tmin, tmax);
}

TransformedShape Light::get_transformed_shape() const {
    return transformed_shape;
}

RgbColor Light::get_base_color() const {
    return base_color;
}

double Light::get_intensity() const {
    return intensity;
}

AreaLight::AreaLight(const TransformedShape& t_shape, const RgbColor& base_color, double intensity)
    : Light{t_shape, base_color, intensity} {}

ShapeSample AreaLight::sample() const {
    return get_transformed_shape().sample_shape();
}

RgbColor AreaLight::compute_emitted_radiance(const Vec3& p, const Vec3& direction) const {
    return get_base_color() * get_intensity();
}

std::shared_ptr<AreaLight> create_area_light(const Vec3& location,
                                             const Vec3& rotation,
                                             const Vec3& scale,
                                             const RgbColor& base_color,
                                             double intensity) {
    TransformedShape shape{primitives.rect_xz, location, rotation, scale};
    return std::make_shared<AreaLight>(shape, base_color, intensity);
}
