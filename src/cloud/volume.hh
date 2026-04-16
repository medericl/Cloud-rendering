#pragma once
#include "../image/color.hh"
#include "../vector/vector.hh"
#include "wind.hh"

Color IntegrateVolume(Point3 origin, Vector3 ray, Color pixel, float t);
Color IntegrateVolume(Point3 origin, Vector3 ray, Color pixel, float t, Wind w);