#include "camera.h"

#include "utils.h"

void Camera::set_pose(const Vec3& location, const Vec3& look_at) {
    set_location(location);
    set_look_at(look_at);
}

Vec3 Camera::v_vec() const {
    return image_plane_height() * normalized_top();
}

Vec3 Camera::u_vec() const {
    return image_plane_width() * normalized_right();
}

Vec3 Camera::image_plane_center() const {
    return location + look_at.normalized() * focal_length;
}

Vec3 Camera::lowerleft_corner() const {
    return image_plane_center() - 0.5 * u_vec() - 0.5 * v_vec();
}

void Camera::focus_on_point(const Vec3& p) {
    set_look_at(normalized(p - location));
}

Ray Camera::generate_ray(double u, double v) const {
    const auto& lowerLeft = lowerleft_corner();
    return {location, lowerLeft + u * u_vec() + v * v_vec() - location};
}

Vec3 Camera::normalized_right() const {
    return cross(look_at, up).normalized();
}

Vec3 Camera::normalized_top() const {
    return cross(normalized_right(), look_at).normalized();
}

double Camera::image_plane_height() const {
    return 2 * focal_length * std::tan(to_radian(vfov) / 2);
}

double Camera::image_plane_width() const {
    return image_plane_height() * aspect_ratio;
}

std::pair<double, double> Camera::to_image_plane_uv(
    int image_width, int image_height, int x, int y, double u, double v) const {
    double img_u = x + u;
    double img_v = image_height - y - 1.0 + v;
    img_u /= image_width;
    img_v /= image_height;
    return {img_u, img_v};
}

std::shared_ptr<Camera> create_camera(const Vec3& location, const Vec3& look_at) {
    return std::make_shared<Camera>(location, look_at);
}