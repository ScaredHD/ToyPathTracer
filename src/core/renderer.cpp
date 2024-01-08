#include "renderer.h"

#include <iomanip>
#include <iostream>

#include "utils.h"

void Renderer::save_output(const std::string& path) const {
    output.save(path);
}

void RayTracer::render(const Camera& camera) {
    // Iterate through all pixels and render for each
    auto [w, h] = std::make_pair(output.get_width(), output.get_height());

    for (int y{}; y < h; ++y) {
        std::cout << "rows remaining: " << std::setw(4) << h - y - 1 << "\r";

        for (int x{}; x < w; ++x) {
            // Render for each pixel
            auto result = Color::black;

            for (size_t s{0}; s < samples_per_pixel; ++s) {
                auto [u_inpix, v_inpix] = pixel_sampler->sample();
                auto [u_img, v_img]     = camera.to_image_plane_uv(w, h, x, y, u_inpix, v_inpix);

                Ray ray = camera.generate_ray(u_img, v_img);
                result += compute_radiance(ray);
            }

            auto d = static_cast<double>(samples_per_pixel);
            output.set_pixel_value(x, y, result / d);
        }
    }
    std::cout << "\ndone.\n";
}
