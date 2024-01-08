#include "utils.h"

#include <gtest/gtest.h>

#include <cmath>

TEST(Utils, Refract) {
    {  // Check total reflection

        double error = 0.05;

        std::vector<double> eta = {1.0, 1.333, 1.52, 2.0, 2.417, 2.65};
        auto len                = eta.size();

        for (size_t i{}; i < 1000; ++i) {
            auto wi = random_vec3(-1.0, 1.0).normalized();
            Vec3 n  = random_vec3(-1.0, 1.0).normalized();

            auto cos_theta_i    = dot(wi, n);
            auto sin_theta_i    = std::sqrt(1 - cos_theta_i * cos_theta_i);
            auto incident_angle = std::acos(absdot(wi, n));
            bool entering       = cos_theta_i > 0;

            double eta_out{};
            double eta_in{};
            double eta_i{};
            double eta_t{};
            double critical{};
            Vec3 wo;

            for (auto j = 0; j < len - 1; ++j) {
                for (auto k = j + 1; k < len; ++k) {
                    eta_out = eta[j];
                    eta_in  = eta[k];

                    eta_i = entering ? eta_out : eta_in;
                    eta_t = entering ? eta_in : eta_out;

                    critical = std::asin(eta_t / eta_i);
                    if (incident_angle > critical) {
                        EXPECT_FALSE(refract(wi, n, eta_out, eta_in, wo));
                    } else {
                        EXPECT_TRUE(refract(wi, n, eta_out, eta_in, wo));
                        double refracted_angle      = std::acos(absdot(wo.normalized(), n));
                        double refracted_angle_true = std::asin(eta_i / eta_t * sin_theta_i);
                        EXPECT_NEAR(refracted_angle, refracted_angle_true, error);
                    }

                    std::swap(eta_out, eta_in);

                    eta_i = entering ? eta_out : eta_in;
                    eta_t = entering ? eta_in : eta_out;

                    critical = std::asin(eta_t / eta_i);
                    if (incident_angle > critical) {
                        EXPECT_FALSE(refract(wi, n, eta_out, eta_in, wo));
                    } else {
                        EXPECT_TRUE(refract(wi, n, eta_out, eta_in, wo));
                        double refracted_angle      = std::acos(absdot(wo.normalized(), n));
                        double refracted_angle_true = std::asin(eta_i / eta_t * sin_theta_i);
                        EXPECT_NEAR(refracted_angle, refracted_angle_true, error);
                    }
                }
            }
        }
    }
    Vec3 normal{0, 1, 0};
    double ior_glass                   = 1.52;
    double ior_air                     = 1.0;
    std::vector<double> incident_angle = {0, 15, 30, 45, 60, 75, 89};

    {  // From air to glass
        std::vector<double> refracted_angle_true = {
            0, 9.80675, 19.21075, 27.7321, 34.7447, 39.4692, 41.14655};

        for (size_t i{}; i < incident_angle.size(); ++i) {
            double ang = to_radian(incident_angle[i]);
            double x   = std::sin(ang);
            double y   = std::cos(ang);
            Vec3 incident{x, y, 0};
            Vec3 wo;
            EXPECT_TRUE(refract(incident, normal, ior_air, ior_glass, wo));
            double refracted_deg = degree_between(-normal, wo);
            EXPECT_NEAR(refracted_deg, refracted_angle_true[i], 0.05);
        }
    }
    {  // From glass to air, normal pointing to glass, positive cos(incident)
        std::vector<double> refracted_angle_true = {0, 23.15935, 49.4446, -1.0, -1.0, -1.0, -1.0};

        for (size_t i{}; i < incident_angle.size(); ++i) {
            double ang = to_radian(incident_angle[i]);
            double x   = std::sin(ang);
            double y   = std::cos(ang);
            Vec3 incident{x, y, 0};
            Vec3 wo;
            if (refract(incident, normal, ior_glass, ior_air, wo)) {
                double refracted_deg = degree_between(-normal, wo);
                EXPECT_NEAR(refracted_deg, refracted_angle_true[i], 0.05);
            } else {
                EXPECT_NEAR(refracted_angle_true[i], -1.0, 1e-3);
            }
        }
    }
    {  // From glass to air, normal pointing to air, negative cos(incident)
        std::vector<double> refracted_angle_true = {0, 23.15935, 49.4446, -1.0, -1.0, -1.0, -1.0};

        for (size_t i{}; i < incident_angle.size(); ++i) {
            double ang = to_radian(incident_angle[i]);
            double x   = std::sin(ang);
            double y   = -std::cos(ang);
            Vec3 incident{x, y, 0};
            Vec3 wo;
            if (refract(incident, normal, ior_air, ior_glass, wo)) {
                double refracted_deg = degree_between(normal, wo);
                EXPECT_NEAR(refracted_deg, refracted_angle_true[i], 0.05);
            } else {
                EXPECT_NEAR(refracted_angle_true[i], -1.0, 1e-3);
            }
        }
    }
}