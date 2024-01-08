#include "image.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Image::load(const std::string& path) {
    auto* img = stbi_load(path.c_str(), &width, &height, &channelCount, channelCount);
    if (!img) {
        std::cerr << "failed to load image file: " << path << "\n";
    }
    auto sz = width * height * channelCount;
    buf.resize(sz);
    std::copy(img, img + sz, buf.data());
    stbi_image_free(img);
}

void Image::save(const std::string& path) const {
    if (!stbi_write_png(path.c_str(), width, height, 3, buf.data(), width * 3)) {
        std::cerr << "failed to write to png\n";
    }
}

void Image::fill(const RgbColor& color) {
    fill_u(to_rgb_u8(color));
}

RgbColor Image::get_pixel_value(int x, int y) const {
    return to_rgb(get_pixel_value_u(x, y));
}

void Image::set_pixel_value(int x, int y, const RgbColor& color) {
    set_pixel_value_u(x, y, to_rgb_u8(color));
}

void Image::fill_u(const RgbColorU8& color) {
    for (int i{}; i < width; ++i) {
        for (int j{}; j < height; ++j) {
            set_pixel_value_u(i, j, color);
        }
    }
}

RgbColorU8 Image::get_pixel_value_u(int x, int y) const {
    auto start = (y * width + x) * 3;
    return {buf[start], buf[start + 1], buf[start + 2]};
}

void Image::set_pixel_value_u(int x, int y, const RgbColorU8& color) {
    auto start     = (y * width + x) * 3;
    buf[start]     = color.r();
    buf[start + 1] = color.g();
    buf[start + 2] = color.b();
}
