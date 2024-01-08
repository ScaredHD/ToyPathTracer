#pragma once

#include <memory>
#include <utility>

#include "camera.h"
#include "image.h"
#include "sampler.h"

class TestScene;

class Renderer {
  public:
    Renderer(int w, int h) : output{w, h} {};
    virtual ~Renderer() = default;

    void load_scene(std::shared_ptr<TestScene> scene) { this->scene = std::move(scene); }


    virtual void render(const Camera& camera) = 0;

    void save_output(const std::string& path) const;

  protected:
    std::shared_ptr<TestScene> scene;
    Image output;
};

class RayTracer : public Renderer {
  public:
    RayTracer(int w, int h, std::shared_ptr<PixelSampler> pixel_sampler, size_t samples_per_pixel)
        : Renderer{w, h},
          pixel_sampler{std::move(pixel_sampler)},
          samples_per_pixel{samples_per_pixel} {}

    ~RayTracer() override = default;

    void render(const Camera& camera) override;

  private:
    virtual RgbColor compute_radiance(const Ray& ray) const = 0;

    std::shared_ptr<PixelSampler> pixel_sampler;
    size_t samples_per_pixel;
};
