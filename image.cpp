#include "image.hh"
#include "color.hh"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>

Image::Image(int w, int h)
    : width(w), height(h), pixels(w * h)
{}

void Image::setPixel(int x, int y, const Color& c) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::runtime_error("wrong widht | height");
        return;
    }
    pixels[y * width + x] = c;
}

const Color& Image::getPixel(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::runtime_error("wrong x | y");
    }
    return pixels[y * width + x];
}

static int b_to_int(float c) {
    //c *= 255.0f;
    if (c > 255)
        c = 255;
    if (c < 0)
        c = 0;
    return static_cast<std::uint8_t>(c);
}

void Image::export_PPM(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) { return; }

    file << "P3\n" << width << ' ' << height << '\n' << "255\n";

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Color& c = pixels[y * width + x];
            file << " " << b_to_int(c.r) << " " << b_to_int(c.g) << " " << b_to_int(c.b) << "    ";
        }
        file << "\n";
    }
}