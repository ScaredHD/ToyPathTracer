#include "pathtracer.h"

#include "bxdf.h"
#include "light.h"
#include "material.h"
#include "objects.h"
#include "scene.h"

RgbColor PathTracer::compute_radiance(const Ray& ray) const {
    auto rec = scene->hit(ray);
    if (!rec.has_value()) {
        return Color::black;
    }

    RgbColor emitted = Color::black;
    if (rec->is_light()) {
        auto light = rec->get_light();
        emitted    = light->compute_emitted_radiance(rec->p, rec->incident);
    }

    auto scattered = compute_scattered_radiance(*rec);
    return emitted + scattered;
}

RgbColor PathTracer::compute_scattered_radiance(const SurfaceIntersection& rec) const {
    if (rec.is_light()) {
        return Color::black;
    }

    RgbColor direct_lighting = compute_direct_lighting(rec);

    RgbColor indirect_lighting = Color::black;
    constexpr double p_rr      = 0.8;
    if (random_double() < p_rr) {
        indirect_lighting = compute_indirect_lighting(rec) / p_rr;
    }

    return direct_lighting + indirect_lighting;
}

RgbColor PathTracer::compute_direct_lighting(const SurfaceIntersection& rec) const {
    auto light = get_random_light(*scene);
    auto bsdf  = rec.get_geometry()->get_material()->compute_bsdf();

    const auto& [world_to_shading, shading_to_world] = shading_transforms(rec.frame);

    // Special case for perfect specular reflection or refraction
    // In this case, we sample BSDF instead of light to get wi
    if (bsdf->type() == BsdfType::specular) {
        auto world_wo   = normalized(rec.incident);
        auto shading_wo = world_to_shading.on_vec(world_wo).normalized();

        auto sample = bsdf->sample(shading_wo);
        if (!sample.has_value()) {
            throw std::runtime_error{"empty sample result for specular BSDF"};
        }

        auto shading_wi = sample->shading_wi;
        auto world_wi   = shading_to_world.on_vec(shading_wi).normalized();

        auto next_rec = scene->hit({rec.p, world_wi});
        if (!next_rec.has_value() || !next_rec->is_light()) {
            return Color::black;
        }

        auto radiance = light->compute_emitted_radiance(next_rec->p, next_rec->incident);
        auto abscos_o = absdot(rec.frame.normal, normalized(next_rec->p - rec.p));

        // double abscos_l      = absdot(-world_wi, next_rec->frame.normal.normalized());
        // double r             = distance(rec.p, next_rec->p);
        // double geometry_term = abscos_o * abscos_l / (r * r);

        // NOTE: BSDF value is (1, 1, 1) and pdf is 1.
        // NOTE: Can't use geometry term like below, why?
        return sample->bsdf_value * radiance * abscos_o / sample->pdf_value;
    }

    // For glossy BSDF, we sample light sources
    const auto& [p, normal, pdf] = light->sample();
    if (!scene->mutually_visible(p, rec.p)) {
        return Color::black;
    }

    auto world_wi = normalized(p - rec.p);
    auto world_wo = normalized(rec.incident);

    auto shading_wo = world_to_shading.on_vec(world_wo).normalized();
    auto shading_wi = world_to_shading.on_vec(world_wi).normalized();
    RgbColor fr     = bsdf->evaluate(shading_wo, shading_wi);

    double abscos_o      = absdot(shading_wi, {0, 1, 0});
    double abscos_l      = absdot(-world_wi, normal.normalized());
    double r             = distance(rec.p, p);
    double geometry_term = abscos_o * abscos_l / (r * r);

    RgbColor radiance = light->compute_emitted_radiance(p, -world_wi);
    return fr * radiance * geometry_term / pdf;
}

RgbColor PathTracer::compute_indirect_lighting(const SurfaceIntersection& rec) const {
    const auto& [world_to_shading, shading_to_world] = shading_transforms(rec.frame);

    // ----------- Get material info -----------
    auto geometry = rec.get_geometry();
    auto material = geometry->get_material();
    if (!material) {
        auto name = geometry->name();
        throw std::runtime_error{"Some " + name + " doesn't have material"};
    }

    // ----------- Transform to shading frame -----------

    auto world_wo   = normalized(rec.incident);
    auto shading_wo = world_to_shading.on_vec(world_wo).normalized();

    // ----------- Sample wo in shading frame -----------

    auto bsdf = material->compute_bsdf();
    if (!bsdf) {
        throw std::runtime_error{"empty BSDF"};
    }

    auto sample = bsdf->sample(shading_wo);
    if (!sample.has_value()) {
        throw std::runtime_error{"Bsdf from " + material->name() + " doesn't sample"};
    }

    // ----------- BSDF value & PDF & cos -----------

    RgbColor bsdf_value = sample->bsdf_value;
    if (is_nearly_black(bsdf_value)) {
        return Color::black;
    }

    auto pdf = sample->pdf_value;

    auto shading_wi = normalized(sample->shading_wi);
    double abscos   = absdot(shading_wi, {0, 1, 0});

    // ----------- Indirect incoming radiance -----------

    auto world_wi = shading_to_world.on_vec(shading_wi).normalized();
    auto next_rec = scene->hit({rec.p, world_wi});
    if (!next_rec.has_value()) {
        return Color::black;
    }

    auto radiance = compute_scattered_radiance(*next_rec);

    return bsdf_value * radiance * abscos / pdf;
}
