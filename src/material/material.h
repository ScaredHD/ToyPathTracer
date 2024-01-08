#pragma once

#include <memory>

#include "bxdf.h"

class Material {
  public:
    virtual ~Material() = default;

    virtual std::shared_ptr<Bsdf> compute_bsdf() const = 0;

    virtual std::string name() const = 0;
};

class MaterialDiffuse : public Material {
  public:
    MaterialDiffuse() = default;
    MaterialDiffuse(const RgbColor& albedo, double reflectance);

    std::shared_ptr<Bsdf> compute_bsdf() const override;

    std::string name() const override { return "diffuse"; }

  private:
    RgbColor albedo{Color::white};
    double reflectance{1.0};
};

class Glass : public Material {
  public:
    Glass() = default;

    std::shared_ptr<Bsdf> compute_bsdf() const override;

    std::string name() const override { return "glass"; }

  private:
    double ior_glass{1.52};
    double ior_out{1.0};
};

class PerfectMirror : public Material {
  public:
    std::shared_ptr<Bsdf> compute_bsdf() const override {
        return std::make_shared<BsdfPerfectMirror>();
    }

    std::string name() const override { return "mirror"; }

  private:
};