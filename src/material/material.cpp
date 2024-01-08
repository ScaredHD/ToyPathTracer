#include "material.h"

MaterialDiffuse::MaterialDiffuse(const RgbColor& albedo, double reflectance)
    : albedo{albedo},
      reflectance{reflectance} {}

std::shared_ptr<Bsdf> MaterialDiffuse::compute_bsdf() const {
    return std::make_shared<BsdfDiffuse>(albedo, reflectance);
}

std::shared_ptr<Bsdf> Glass::compute_bsdf() const {
    auto fresnel{std::make_shared<FresnelDielectrics>()};
    return std::make_shared<BsdfPerfectSpecular>(fresnel, ior_out, ior_glass);
}
