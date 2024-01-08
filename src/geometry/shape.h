#pragma once

#include <optional>
#include <utility>

#include "intersection.h"
#include "ray.h"
#include "sampler.h"
#include "transform.h"
#include "vec.h"

struct ShapeSample {
    Vec3 p;
    Vec3 normal;
    double pdf_value;
};

void transform_shape_sample(const Transform& transform, ShapeSample& sample);

// Shape is anything that can be hit by ray
class Shape {
  public:
    virtual ~Shape() = default;

    virtual std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const = 0;

    virtual ShapeSample sample_shape() const = 0;

    virtual double compute_area() const = 0;

    virtual std::string name() const = 0;
};

class TransformedShape {
  public:
    TransformedShape(std::shared_ptr<Shape> shape);

    TransformedShape(std::shared_ptr<Shape> shape, const Transform& transform);

    TransformedShape(std::shared_ptr<Shape> shape,
                     const Vec3& location,
                     const Vec3& rotation,
                     const Vec3& scale);

    std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const;

    ShapeSample sample_shape() const;

    double compute_area() const;

    std::shared_ptr<Shape> get_shape() const { return shape; }

    Transform get_transform() const { return local_to_world; }

    std::string name() const { return shape->name(); }

  private:
    std::shared_ptr<Shape> shape;
    Transform local_to_world;
};

// Unit sphere centered at (0, 0, 0) with radius 1.
class Sphere : public Shape {
  public:
    Sphere() = default;

    std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const override;

    ShapeSample sample_shape() const override;

    double compute_area() const override { return 4 * pi * pi; }

    std::string name() const override { return "Sphere"; }

  private:
    Vec3 center{Vec3::zero()};
    double radius{1.0};
};

// Unit rectangle with side length of 1 centered at (0, 0)
class RectXZ : public Shape {
  public:
    RectXZ() = default;

    std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const override;

    ShapeSample sample_shape() const override;

    double compute_area() const override { return 1.; }

    bool inside(double x, double z) const { return (x > x0) && (x < x1) && (z > z0) && (z < z1); }

    std::string name() const override { return "RectXZ"; }

  private:
    double x0{-0.5};
    double z0{-0.5};
    double x1{0.5};
    double z1{0.5};
};

struct Primitive {
    Primitive();

    std::shared_ptr<Sphere> sphere;
    std::shared_ptr<RectXZ> rect_xz;
};

const extern Primitive primitives;