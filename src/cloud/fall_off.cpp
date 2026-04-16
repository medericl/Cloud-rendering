#include "fall_off.hh"
#include "../scene/config.hh"
#include <cmath>

float smooth_step(float a, float b, float val)
{
    float t = std::max(0.0f, std::min(1.0f, (val - a) / (b - a)));
    return t * t * (3.0f - 2.0f * t);     // 3t^23 - 2t^2
}

float very_smooth_step(float a, float b, float val)
{
    float t = std::max(0.0f, std::min(1.0f, (val - a) / (b - a)));
    float t3 = t * t * t;
    float t4 = t3 * t;
    float t5 = t4 * t;
    return (6 * t5) - (15 * t4) + (10 * t3);     // 6t⁵ - 15t⁴ + 10t³
}

float vertical_falloff(float py)
{
    float height = BOX_MAX_Y - BOX_MIN_Y;
    float h = (py - BOX_MIN_Y) / height;
    h = std::max(0.0f, std::min(1.0f, h));  // clamp [0,1]

    float bottom = smooth_step(0.0f, 0.1f, h);
    float top = smooth_step(1.0f, 0.6f, h);
    return bottom * top;
}

float rev_vertical_falloff(float py)
{
    float height = BOX_MAX_Y - BOX_MIN_Y;
    float h = (py - BOX_MIN_Y) / height;
    h = std::min(std::max(h, -2.0f), 1.0f);
    float falloff = std::pow((h), 1.8f);
    return falloff;
}
