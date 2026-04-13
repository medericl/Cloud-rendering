#ifndef COLOR_HPP
#define COLOR_HPP

#include <ostream>

class Color {
public:
    float r;
    float g;
    float b;

    Color() : r(0), g(0), b(0) { }

    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    Color operator*(float t) const {
        return Color(r * t, g * t, b * t);
    };

    Color operator+(Color c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    };

    bool is_black() const {
        return r == 0 && g == 0 && b == 0;
    }

};

inline std::ostream& operator<<(std::ostream& os, const Color& c) {
    os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}


#endif
