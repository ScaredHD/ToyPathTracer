#pragma once

#include "mat.h"
#include "vec.h"

class Ray;

class Transform {
  public:
    Transform() = default;
    Transform(const Vec3& t, const Vec3& r, const Vec3& s);
    Transform(const Mat4& mat);

    Transform operator*(const Transform& rhs) const;

    Mat4 get_mat() const { return mat; }
    Mat4 get_inv_mat() const { return inv_mat; }
    Transform inverse() const { return {inv_mat}; }

    [[nodiscard]] Vec3 on_point(const Vec3& p) const;
    [[nodiscard]] Vec3 on_vec(const Vec3& v) const;
    [[nodiscard]] Vec3 on_normal(const Vec3& n) const;
    [[nodiscard]] Ray on_ray(const Ray& r) const;

    void apply_on_point(Vec3& p) const;
    void apply_on_vec(Vec3& v) const;
    void apply_on_normal(Vec3& n) const;
    void apply_on_light(Ray& r) const;

    Vec3 extract_sacle() const;
    bool is_uniform_scale() const;
    double uniform_scaling_factor() const;

  private:
    Mat4 mat;
    Mat4 inv_mat;
};


Mat4 translate(const Vec3& displacement);

Mat4 rotate_x(double deg);

Mat4 rotate_y(double deg);

Mat4 rotate_z(double deg);

Mat4 rotate(const Vec3& degrees);

Mat4 scale(const Vec3& scales);