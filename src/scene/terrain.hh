#pragma once
#include "../vector/vector.hh"
#include "../image/color.hh"

float terrain_height(float x, float z);
bool terrain_hit(Point3 origin, Vector3 ray, float& t_hit, Color& col);
