#include "sampler.h"

#include <gtest/gtest.h>

#include <vector>

TEST(Sampler, SamplerTest) {
    Sampler sampler;

    int sample_count{1000000};
    int bucket_count{20};
    double bucket_width{1.0 / bucket_count};

    std::vector<int> buckets(bucket_count, 0);

    for (int i{}; i < sample_count; ++i) {
        double x{sampler.next_1d()};
        int idx{static_cast<int>(x / bucket_width)};
        ++buckets[idx];
    }

    double prob_each_bucket{1.0 / bucket_count};
    for (int i{}; i < bucket_count; ++i) {
        double prob{(1.0 * buckets[i]) / sample_count};
        EXPECT_NEAR(prob, prob_each_bucket, 0.001);
    }
}