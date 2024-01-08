#include "shape.h"

#include <cmath>

TransformedShape::TransformedShape(std::shared_ptr<Shape> shape,
                                   const Vec3& location,
                                   const Vec3& rotation,
                                   const Vec3& scale)
    : shape{std::move(shape)},
      local_to_world{location, rotation, scale} {}

TransformedShape::TransformedShape(std::shared_ptr<Shape> shape) : shape{std::move(shape)} {}

TransformedShape::TransformedShape(std::shared_ptr<Shape> shape, const Transform& transform)
    : shape{std::move(shape)},
      local_to_world{transform} {}

std::optional<SurfaceIntersection> TransformedShape::hit(const Ray& ray, double tmin, double tmax) const {
    Transform inv_t = local_to_world.inverse();
    Ray inv_ray     = inv_t.on_ray(ray);

    std::optional<SurfaceIntersection> rec = shape->hit(inv_ray, tmin, tmax);
    if (!rec.has_value()) {
        return std::nullopt;
    }

    transform_hit_record(local_to_world, rec.value());

    // This is important, since transformation may change length of vector
    rec->frame.normal.normalize();
    rec->frame.tangent.normalize();
    rec->frame.bitangent.normalize();

    return rec;
}

ShapeSample TransformedShape::sample_shape() const {
    auto sample = shape->sample_shape();
    // XXX: Need refactor: transform shape sample doesn't actually touch member 'pdf_value'
    transform_shape_sample(local_to_world, sample);
    sample.pdf_value = 1.0 / compute_area();
    return sample;
}

double TransformedShape::compute_area() const {
    double s = local_to_world.uniform_scaling_factor();
    return s * s * shape->compute_area();
}

std::optional<SurfaceIntersection> Sphere::hit(const Ray& ray, double tmin, double tmax) const {
    const auto& oc = ray.o - center;
    const auto& a  = dot(ray.d, ray.d);
    const auto& h  = dot(ray.d, oc);
    const auto& c  = dot(oc, oc) - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant <= 0.0) {
        return std::nullopt;
    }

    auto t = -(h + std::sqrt(discriminant)) / a;
    if (t < tmin || t > tmax) {
        // Hit point with smaller t is not in [tmin, tmax], check another root
        t = (-h + std::sqrt(discriminant)) / a;
        if (t < tmin || t > tmax) {
            return std::nullopt;
        }
    }

    SurfaceIntersection rec;
    rec.t        = t;
    rec.p        = ray.at(t);
    rec.incident = -ray.d;
    rec.frame    = generate_world_shading_frame(rec.p - center);
    return rec;
}

std::optional<SurfaceIntersection> RectXZ::hit(const Ray& ray, double tmin, double tmax) const {
    auto [o, d] = std::make_tuple(ray.o, ray.d);
    if (is_nearly_zero(d.y())) {
        return std::nullopt;
    }

    double t = -o.y() / d.y();
    if (t < tmin || t > tmax) {
        return std::nullopt;
    }

    double x = o.x() + t * d.x();
    double z = o.z() + t * d.z();
    if (!inside(x, z)) {
        return std::nullopt;
    }

    SurfaceIntersection rec;
    rec.t        = t;
    rec.p        = ray.at(t);
    rec.incident = -ray.d;
    rec.frame    = generate_world_shading_frame({0, 1, 0});
    return rec;
}

Primitive::Primitive() {
    sphere  = std::make_shared<Sphere>();
    rect_xz = std::make_shared<RectXZ>();
}

const Primitive primitives{};

void transform_shape_sample(const Transform& transform, ShapeSample& sample) {
    transform.apply_on_point(sample.p);
    transform.apply_on_normal(sample.normal);
}

ShapeSample RectXZ::sample_shape() const {
    auto [x, z] = Sampler{}.next_2d();
    Vec3 p{x - 0.5, 0, z - 0.5};
    return {p, {0, 1, 0}, 1.0 / compute_area()};
}

ShapeSample Sphere::sample_shape() const {
    // TODO: Implement
    return {};
}
