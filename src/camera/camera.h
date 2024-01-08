#pragma once

#include <memory>

#include "ray.h"
#include "vec.h"

// Image plane coordinates
// origin: top-left
// x: right
// y: down

class Camera {
  public:
    Camera(Vec3 location,
           Vec3 look_at,
           Vec3 up             = {0.0, 1.0, 0.0},
           double aspect_ratio = 1.0,
           double focal_length = 1.0,
           double vfov         = 60.0)
        : location{location},
          look_at{look_at},
          up{up},
          aspect_ratio{aspect_ratio},
          focal_length{focal_length},
          vfov{vfov} {}

    Vec3 get_location() const { return location; }

    void set_location(const Vec3& location) { this->location = location; }

    Vec3 get_look_at() const { return look_at; }

    void set_look_at(const Vec3& look_at) { this->look_at = look_at; }

    void set_pose(const Vec3& location, const Vec3& look_at);

    void set_aspect_ratio(double aspectRatio) { this->aspect_ratio = aspectRatio; }

    void set_vfov(double vFov) { this->vfov = vFov; }

    void focus_on_point(const Vec3& p);

    Ray generate_ray(double u, double v) const;

    double image_plane_width() const;
    double image_plane_height() const;

    std::pair<double, double> to_image_plane_uv(
        int image_width, int image_height, int x, int y, double u, double v) const;

  private:
    Vec3 normalized_right() const;
    Vec3 normalized_top() const;
    Vec3 u_vec() const;
    Vec3 v_vec() const;
    Vec3 image_plane_center() const;
    Vec3 lowerleft_corner() const;

    Vec3 location;
    Vec3 look_at;  // direction
    Vec3 up{0.0, 1.0, 0.0};
    double aspect_ratio{1.0};
    double focal_length{1.0};
    double vfov{60.0};
};

std::shared_ptr<Camera> create_camera(const Vec3& location, const Vec3& look_at);