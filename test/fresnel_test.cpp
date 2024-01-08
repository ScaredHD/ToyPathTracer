#include <gtest/gtest.h>

#include <cmath>

#include "bxdf.h"
#include "utils.h"

TEST(FresnelDielectrics, ReflectancePositiveCos) {
    double error{0.00005};

    double a1{std::cos(to_radian(5))};
    double a2{std::cos(to_radian(30))};
    double a3{std::cos(to_radian(60))};
    double a4{std::cos(to_radian(75))};
    double a5{std::cos(to_radian(88))};

    FresnelDielectrics f;

    EXPECT_NEAR(f.reflectance(a1, 1.0, 1.333), 0.0204, error);
    EXPECT_NEAR(f.reflectance(a2, 1.0, 1.333), 0.0214, error);
    EXPECT_NEAR(f.reflectance(a3, 1.0, 1.333), 0.0597, error);
    EXPECT_NEAR(f.reflectance(a4, 1.0, 1.333), 0.2124, error);
    EXPECT_NEAR(f.reflectance(a5, 1.0, 1.333), 0.8040, error);

    EXPECT_NEAR(f.reflectance(a1, 1.0, 2.417), 0.1720, error);
    EXPECT_NEAR(f.reflectance(a2, 1.0, 2.417), 0.1735, error);
    EXPECT_NEAR(f.reflectance(a3, 1.0, 2.417), 0.2113, error);
    EXPECT_NEAR(f.reflectance(a4, 1.0, 2.417), 0.3305, error);
    EXPECT_NEAR(f.reflectance(a5, 1.0, 2.417), 0.8141, error);
}

TEST(FresnelDielectrics, ReflectanceNegativeCos) {
    double error{0.0005};

    double a1{std::cos(to_radian(95))};   // 85
    double a2{std::cos(to_radian(105))};  // 75
    double a3{std::cos(to_radian(135))};  // 45
    double a4{std::cos(to_radian(155))};  // 25
    double a5{std::cos(to_radian(175))};  // 5

    FresnelDielectrics f;

    EXPECT_NEAR(f.reflectance(a1, 1.0, 1.333), 1.0, error);
    EXPECT_NEAR(f.reflectance(a2, 1.0, 1.333), 1.0, error);
    EXPECT_NEAR(f.reflectance(a3, 1.0, 1.333), 0.1395, error);
    EXPECT_NEAR(f.reflectance(a4, 1.0, 1.333), 0.0223, error);
    EXPECT_NEAR(f.reflectance(a5, 1.0, 1.333), 0.0204, error);

    EXPECT_NEAR(f.reflectance(a1, 1.0, 2.417), 1.0, error);
    EXPECT_NEAR(f.reflectance(a2, 1.0, 2.417), 1.0, error);
    EXPECT_NEAR(f.reflectance(a3, 1.0, 2.417), 1.0, error);
    EXPECT_NEAR(f.reflectance(a4, 1.0, 2.417), 1.0, error);
    EXPECT_NEAR(f.reflectance(a5, 1.0, 2.417), 0.1720, error);
}