#include <gtest/gtest.h>

#include "shape.h"
#include "transform.h"

TEST(Transform, Point) {
    // clang-format off
    std::vector<Vec3> p {
        {-1, 0, -1},  // p0
        {-1, 0,  0},  // p1
        {-1, 0,  1},  // p2
        { 0, 0, -1},  // p3
        { 0, 0,  0},  // p4
        { 0, 0,  1},  // p5
        { 1, 0, -1},  // p6
        { 1, 0,  0},  // p7
        { 1, 0,  1},  // p8
    };
    // clang-format on

    // -------- Rotate Y 90 deg --------
    // clang-format off
    std::vector<Vec3> q1 {
        {-1, 0,  1},
        { 0, 0,  1},
        { 1, 0,  1},
        {-1, 0,  0},
        { 0, 0,  0},
        { 1, 0,  0},
        {-1, 0, -1},
        { 0, 0, -1},
        { 1, 0, -1},
    };
    // clang-format on

    Transform rotate_y_90{{0, 0, 0}, {0, 90, 0}, {1, 1, 1}};

    for (size_t i{}; i < 8; ++i) {
        auto pp{rotate_y_90.on_point(p[i])};
        EXPECT_TRUE(are_nearly_equal(pp, q1[i]));
    }

    // -------- Rotate X 90 deg --------
    // clang-format off
    std::vector<Vec3> q2 {
        { -1,  1, 0},
        { -1,  0, 0},
        { -1, -1, 0},
        {  0,  1, 0},
        {  0,  0, 0},
        {  0, -1, 0},
        {  1,  1, 0},
        {  1,  0, 0},
        {  1, -1, 0},
    };
    // clang-format on

    Transform rotate_x_90{{0, 0, 0}, {90, 0, 0}, {1, 1, 1}};

    for (size_t i{}; i < 8; ++i) {
        auto pp{rotate_x_90.on_point(p[i])};
        EXPECT_TRUE(are_nearly_equal(pp, q2[i]));
    }

    // -------- Scale X 2x --------
    // clang-format off
    std::vector<Vec3> q3 {
        {-2, 0, -1},  // p0
        {-2, 0,  0},  // p1
        {-2, 0,  1},  // p2
        { 0, 0, -1},  // p3
        { 0, 0,  0},  // p4
        { 0, 0,  1},  // p5
        { 2, 0, -1},  // p6
        { 2, 0,  0},  // p7
        { 2, 0,  1},  // p8
    };
    // clang-format on

    Transform scale_x_2{{0, 0, 0}, {0, 0, 0}, {2, 1, 1}};

    for (size_t i{}; i < 8; ++i) {
        auto pp{scale_x_2.on_point(p[i])};
        EXPECT_TRUE(are_nearly_equal(pp, q3[i]));
    }

    // -------- Scale XYZ 3x --------
    // clang-format off
    std::vector<Vec3> q4 {
        {-3, 0, -3},  // p0
        {-3, 0,  0},  // p1
        {-3, 0,  3},  // p2
        { 0, 0, -3},  // p3
        { 0, 0,  0},  // p4
        { 0, 0,  3},  // p5
        { 3, 0, -3},  // p6
        { 3, 0,  0},  // p7
        { 3, 0,  3},  // p8
    };
    // clang-format on
    Transform scale_xyz_3{{0, 0, 0}, {0, 0, 0}, {3, 3, 3}};

    for (size_t i{}; i < 8; ++i) {
        auto pp{scale_xyz_3.on_point(p[i])};
        EXPECT_TRUE(are_nearly_equal(pp, q4[i]));
    }
}

bool are_nearly_equal(const Transform& t1, const Transform& t2) {
    return are_nearly_equal(t1.get_mat(), t2.get_mat());
}

TEST(Transform, TransformMatConversion) {
    Vec3 loc{1, 2, 3};
    Vec3 rot{42, 10, 35};
    Vec3 scale{2, 3, 4};
    Transform t1{loc, rot, scale};

    Mat4 m1{t1.get_mat()};
    // clang-format off
    Mat4 m1true{
          1.61341457, -0.993211119,  1.95802254, 1,
          1.12972504,   2.02618304, -1.89640842, 2,
        -0.347296355,	1.97689503,  2.92741914, 3,
                   0,	         0,           0, 1
    };
    // clang-format on
    EXPECT_TRUE(are_nearly_equal(m1, m1true));
}