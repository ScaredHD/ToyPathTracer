#pragma once

#include <memory>
#include <optional>

#include "fresnel.h"
#include "utils.h"
#include "vec.h"

enum class BsdfType : unsigned char {
    none       = 0x00,
    diffuse    = 0x01,
    glossy     = 0x02,
    specular   = 0x04,
    reflection = 0x08,
    refraction = 0x10
};

struct BsdfSample {
    Vec3 shading_wi{};
    double pdf_value{1.0};
    RgbColor bsdf_value{Color::black};
};

class Bsdf {
  public:
    virtual ~Bsdf() = default;

    virtual std::optional<BsdfSample> sample(const Vec3& shading_wo) const = 0;

    virtual RgbColor evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const = 0;

    virtual BsdfType type() const = 0;
};

class BsdfPerfectMirror : public Bsdf {
    std::optional<BsdfSample> sample(const Vec3& shading_wo) const override;

    RgbColor evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const override;

    BsdfType type() const override { return BsdfType::specular; }
};

class BsdfPerfectSpecular : public Bsdf {
  public:
    BsdfPerfectSpecular() = delete;

    BsdfPerfectSpecular(std::shared_ptr<Fresnel> fresnel, double eta_out, double eta_in);

    std::optional<BsdfSample> sample(const Vec3& shading_wo) const override;

    RgbColor evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const override;

    BsdfType type() const override { return BsdfType::specular; }

  private:
    std::shared_ptr<Fresnel> fresnel;
    double eta_out;
    double eta_in;
};

class BsdfDiffuse : public Bsdf {
  public:
    BsdfDiffuse() = default;

    BsdfDiffuse(const RgbColor& albedo, double reflectance);

    std::optional<BsdfSample> sample(const Vec3& shading_wo) const override;

    RgbColor evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const override;

    BsdfType type() const override { return BsdfType::diffuse; }

  private:
    RgbColor albedo{Color::white};
    double reflecance{0.8};
};