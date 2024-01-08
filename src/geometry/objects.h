#pragma once

#include <utility>

#include "shape.h"

class Material;

class Geometry {
  public:
    Geometry(std::shared_ptr<Shape> shape) : Geometry{std::move(shape), nullptr} {}

    Geometry(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material)
        : transformed_shape{std::make_shared<TransformedShape>(shape)},
          material{std::move(material)} {}

    Geometry(std::shared_ptr<Shape> shape,
             std::shared_ptr<Material> material,
             const Transform& transform)
        : transformed_shape{std::make_shared<TransformedShape>(shape, transform)},
          material{std::move(material)} {}

    Geometry(std::shared_ptr<Shape> shape,
             std::shared_ptr<Material> material,
             const Vec3& location,
             const Vec3& rotation,
             const Vec3& scale)
        : transformed_shape{std::make_shared<TransformedShape>(shape, location, rotation, scale)},
          material{std::move(material)} {}

    virtual ~Geometry() = default;

    std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const {
        return transformed_shape->hit(ray, tmin, tmax);
    }

    std::string name() const;

    std::shared_ptr<TransformedShape> get_transformed_shape() const { return transformed_shape; }

    std::shared_ptr<Material> get_material() const { return material; }

  private:
    std::shared_ptr<TransformedShape> transformed_shape{};
    std::shared_ptr<Material> material{};
};

std::shared_ptr<Geometry> create_geometry(std::shared_ptr<Shape> shape,
                                          std::shared_ptr<Material> material,
                                          const Vec3& location,
                                          const Vec3& rotation,
                                          const Vec3& scale);