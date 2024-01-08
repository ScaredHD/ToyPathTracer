#include "sampler.h"

#include "utils.h"

std::tuple<double, double> PixelSampler::sample() const {
    return sampler->next_2d();
}

std::tuple<double, double, double> HemisphericalSampler::sample() const {
    auto [ksi1, ksi2] = sampler->next_2d();
    double theta      = std::acos(ksi1);
    double phi        = 2 * pi * ksi2;
    return {theta, phi, inv_2pi};
}
