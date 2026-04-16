#pragma once
#include "../vector/vector.hh"

float fbm(Point3 p, int octave);
float noise(Point3 p);
float remap_smooth(float val, float low, float high, float min, float max);