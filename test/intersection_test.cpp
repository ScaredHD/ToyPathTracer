#include "intersection.h"

#include <gtest/gtest.h>

#include "utils.h"

TEST(Intersection, GenerateShadingFrame) {
    for (size_t i{}; i < 1000; ++i) {
        Vec3 normal    = random_vec3(-1, 1);
        auto frame     = generate_world_shading_frame(normal);
        auto [n, t, b] = std::make_tuple(frame.normal, frame.tangent, frame.bitangent);
        auto nt        = dot(n, t);
        auto tb        = dot(t, b);
        auto nb        = dot(n, b);
        EXPECT_NEAR(nt, 0.0, 1e-6);
        EXPECT_NEAR(tb, 0.0, 1e-6);
        EXPECT_NEAR(nb, 0.0, 1e-6);
    }
}

TEST(Intersection, ShadingTrasform) {
    Vec3 n     = random_vec3(-1, 1);
    auto frame = generate_world_shading_frame(n);

    auto [world_to_shading, shading_to_world] = shading_transforms(frame);

    auto m1 = world_to_shading * shading_to_world;
    auto m2 = shading_to_world * world_to_shading;
    auto id = Mat4::identity();

    EXPECT_TRUE(are_nearly_equal(m1.get_mat(), id));
    EXPECT_TRUE(are_nearly_equal(m2.get_mat(), id));

    for (size_t i{}; i < 1000; ++i) {
        Vec3 world_wo = random_vec3(-1, 1).normalized();

        Vec3 shading_wo = world_to_shading.on_vec(world_wo).normalized();
        Vec3 world_wo2  = shading_to_world.on_vec(shading_wo).normalized();

        double dist = distance(world_wo, world_wo2);
        EXPECT_NEAR(dist, 0, 1e-6);
    }
}