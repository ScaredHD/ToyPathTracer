#include "bxdf.h"

#include <utility>

#include "sampler.h"

BsdfPerfectSpecular::BsdfPerfectSpecular(std::shared_ptr<Fresnel> fresnel,
                                         double eta_out,
                                         double eta_in)
    : fresnel{std::move(fresnel)},
      eta_out{eta_out},
      eta_in{eta_in} {}

std::optional<BsdfSample> BsdfPerfectSpecular::sample(const Vec3& shading_wo) const {
    BsdfSample res{};

    Vec3 normal{0, 1, 0};
    Vec3 wo = shading_wo.normalized();

    Vec3 reflect_dir{-wo.x(), wo.y(), -wo.z()};
    Vec3 refract_dir;

    if (!refract(wo, normal, eta_out, eta_in, refract_dir)) {
        // Sample total reflection
        res.shading_wi = reflect_dir;
        res.bsdf_value = Color::white;
        res.pdf_value  = 1.0;
        return res;
    }

    refract_dir.normalize();

    // ----------- Choose to sample reflection or refraction -----------
    double cos_theta_in = wo.y();

    double pr = fresnel->reflectance(cos_theta_in, eta_out, eta_in);
    double pt = 1 - pr;
    if (random_double() < pr) {
        // Sample reflection
        res.shading_wi = reflect_dir;
        res.bsdf_value = pr * Color::white / absdot(res.shading_wi, normal);
        res.pdf_value  = pr;
    } else {
        // Sample refraction
        res.shading_wi = refract_dir;
        res.bsdf_value = pt * Color::white / absdot(res.shading_wi, normal);
        pt /= (cos_theta_in < 0 ? eta_in / eta_out : eta_out / eta_in);   // Account for asymmetry
        res.pdf_value  = pt;
    }

    return res;
}

RgbColor BsdfPerfectSpecular::evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const {
    return Color::black;
}

BsdfDiffuse::BsdfDiffuse(const RgbColor& albedo, double reflectance)
    : albedo{albedo},
      reflecance{reflectance} {}

std::optional<BsdfSample> BsdfDiffuse::sample(const Vec3& shading_wo) const {
    BsdfSample res{};
    auto [theta, phi, pdf]   = HemisphericalSampler{}.sample();
    CartesianCoordinates dir = SphericalCoordinates{1, theta, phi};

    res.shading_wi = dir.to_vec().normalized();
    res.pdf_value  = pdf;
    res.bsdf_value = evaluate(shading_wo, res.shading_wi);
    return res;
}

RgbColor BsdfDiffuse::evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const {
    return albedo * (reflecance / pi);
}

std::optional<BsdfSample> BsdfPerfectMirror::sample(const Vec3& shading_wo) const {
    BsdfSample res{};
    Vec3 wo        = shading_wo.normalized();
    res.shading_wi = {-wo.x(), wo.y(), -wo.z()};
    res.bsdf_value = Color::white;
    res.pdf_value  = 1.0;
    return res;
}

RgbColor BsdfPerfectMirror::evaluate(const Vec3& shading_wo, const Vec3& shading_wi) const {
    return Color::black;
}