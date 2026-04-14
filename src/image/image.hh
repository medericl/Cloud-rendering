#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "color.hh"

class Image {
public:
    Image(int width, int height); 

    int width;
    int height;
    std::vector<Color> pixels;

    void setPixel(int x, int y, const Color& c);
    const Color& getPixel(int x, int y);

    void export_PPM(const std::string& filename);
};

#endif
