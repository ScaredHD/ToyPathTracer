#include "objects.h"

std::shared_ptr<Geometry> create_geometry(std::shared_ptr<Shape> shape,
                                                 std::shared_ptr<Material> material,
                                                 const Vec3& location,
                                                 const Vec3& rotation,
                                                 const Vec3& scale) {
    return std::make_shared<Geometry>(shape, material, location, rotation, scale);
}

std::string Geometry::name() const {
    return transformed_shape->name();
}
