#include "effect.hh"
#include "../scene/config.hh"
#include <cmath>

float vertical_falloff(float py)
{
    float height = BOX_MAX_Y - BOX_MIN_Y;
    float h = (py - BOX_MIN_Y) / height;
    h = std::min(std::max(h, -2.0f), 1.0f);
    float falloff = std::pow((1 - h), 1.8f);
    return falloff;
}

float rev_vertical_falloff(float py)
{
    float height = BOX_MAX_Y - BOX_MIN_Y;
    float h = (py - BOX_MIN_Y) / height;
    h = std::min(std::max(h, -2.0f), 1.0f);
    float falloff = std::pow((h), 1.8f);
    return falloff;
}
