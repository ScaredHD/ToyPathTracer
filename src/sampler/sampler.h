#pragma once

#include <chrono>
#include <cstddef>
#include <memory>
#include <random>
#include <utility>

/**
 * @brief Generating uniform real random variable in [0, 1]^inf space,
 *        that is, sequence (x0, x1, x2, x3, ...) where each x is in [0, 1].
 */
class Sampler {
  public:
    Sampler()
        : seed{std::chrono::high_resolution_clock::now().time_since_epoch().count()},
          gen{static_cast<unsigned int>(seed)},
          dist{0.0, 1.0} {}

    double next_1d() { return dist(gen); }

    std::tuple<double, double> next_2d() { return {dist(gen), dist(gen)}; }

    void discard() { dist(gen); }

  private:
    long long seed;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;  // FIXME: This is [0, 1)
};

class SquareSampler {
  public:
    std::tuple<double, double> sample() const;

  private:
    std::shared_ptr<Sampler> sampler;
};

class DiskSampler {
  public:
    std::tuple<double, double> sample() const;

  private:
    std::shared_ptr<Sampler> sampler;
};

class SphereSampler {
  public:
    std::tuple<double, double> sample() const;

  private:
    std::shared_ptr<Sampler> sampler;
};

class HemisphericalSampler {
  public:
    HemisphericalSampler() : sampler{std::make_shared<Sampler>()} {}

    // Return <theta, phi, pdf> sample direction and pdf
    std::tuple<double, double, double> sample() const;

  private:
    std::shared_ptr<Sampler> sampler;
};

class PixelSampler {
  public:
    PixelSampler() : sampler{std::make_shared<Sampler>()} {}

    std::tuple<double, double> sample() const;

  private:
    std::shared_ptr<Sampler> sampler{};
};