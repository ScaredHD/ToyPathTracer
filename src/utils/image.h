#pragma once

#include <vector>
#include <string>

#include "color.h"

// Origin: top-left
// x-axis: right
// y-axis: down
class Image {
  public:
    Image(int w, int h) : width{w}, height{h}, buf(w * h * channelCount) {}
    Image(const std::string& path) { load(path); }

    void load(const std::string& path);
    void save(const std::string& path) const;

    int get_width() const { return width; }
    int get_height() const { return height; }

    void fill(const RgbColor& color);
    void fill_u(const RgbColorU8& color);

    RgbColor get_pixel_value(int x, int y) const;
    RgbColorU8 get_pixel_value_u(int x, int y) const;
    
    void set_pixel_value(int x, int y, const RgbColor& color);
    void set_pixel_value_u(int x, int y, const RgbColorU8& color);

  private:
    int width{};
    int height{};
    int channelCount{3};
    std::vector<unsigned char> buf{};
};
