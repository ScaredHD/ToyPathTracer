#pragma once

#include <utility>

#include "bxdf.h"
#include "intersection.h"
#include "renderer.h"

class PathTracer : public RayTracer {
  public:
    PathTracer(int w, int h, std::shared_ptr<PixelSampler> pixel_sampler, size_t samples_per_pixel)
        : RayTracer{w, h, std::move(pixel_sampler), samples_per_pixel} {}

    RgbColor compute_radiance(const Ray& ray) const override;

  private:
    RgbColor compute_scattered_radiance(const SurfaceIntersection& rec) const;
    RgbColor compute_direct_lighting(const SurfaceIntersection& rec) const;
    RgbColor compute_indirect_lighting(const SurfaceIntersection& rec) const;
};