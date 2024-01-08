#include "shape.h"

#include <gtest/gtest.h>

#include "transform.h"

TEST(Shape, RectXZ) {
    RectXZ rect;
    Ray r1{{0, 1, 0}, {0, -1, 0}};
    Ray r2{{0.49, 1, 0.49}, {0, -1, 0}};
    Ray r3{{-0.25, 1, 0.2}, {0, -1, 0}};
    Ray r4{{0.1, 1, -0.1}, {0, -1, 0}};

    EXPECT_TRUE(rect.hit(r1, 0.01, inf));
    EXPECT_TRUE(rect.hit(r2, 0.01, inf));
    EXPECT_TRUE(rect.hit(r3, 0.01, inf));
    EXPECT_TRUE(rect.hit(r4, 0.01, inf));

    Ray s1{{1, 1, 0}, {0, -1, 0}};
    Ray s2{{2, 1, 0.3}, {0, -1, 0}};
    Ray s3{{-0.5, 1, -1.5}, {0, -1, 0}};
    Ray s4{{0, 1, 1}, {0, -1, 0}};

    EXPECT_FALSE(rect.hit(s1, 0.01, inf));
    EXPECT_FALSE(rect.hit(s2, 0.01, inf));
    EXPECT_FALSE(rect.hit(s3, 0.01, inf));
    EXPECT_FALSE(rect.hit(s4, 0.01, inf));

    TransformedShape big_rect{primitives.rect_xz, {0, 0, 0}, {0, 0, 0}, Vec3::all(1000)};
    Ray r5{{499, 1, 499}, {0, -1, 0}};
    Ray r6{{-499, 1, 499}, {0, -1, 0}};
    Ray r7{{499, 1, -499}, {0, -1, 0}};
    Ray r8{{-499, 1, -499}, {0, -1, 0}};

    EXPECT_TRUE(big_rect.hit(r5, 0.01, inf));
    EXPECT_TRUE(big_rect.hit(r6, 0.01, inf));
    EXPECT_TRUE(big_rect.hit(r7, 0.01, inf));
    EXPECT_TRUE(big_rect.hit(r8, 0.01, inf));

    Ray s9{{501, 1, 499}, {0, -1, 0}};
    Ray s10{{-501, 1, 499}, {0, -1, 0}};
    Ray s11{{501, 1, -499}, {0, -1, 0}};
    Ray s12{{-501, 1, -501}, {0, -1, 0}};

    EXPECT_FALSE(big_rect.hit(s9, 0.01, inf));
    EXPECT_FALSE(big_rect.hit(s10, 0.01, inf));
    EXPECT_FALSE(big_rect.hit(s11, 0.01, inf));
    EXPECT_FALSE(big_rect.hit(s12, 0.01, inf));
}
