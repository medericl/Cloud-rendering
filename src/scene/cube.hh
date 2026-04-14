#pragma once

#include "../vector/vector.hh"
#include "../image/color.hh"

class Cube {
public:
    // half_extents : demi-largeur, demi-hauteur, demi-profondeur
    Cube(const Point3& center, const Vector3& half_extents, const Color& color);

    Point3 center;
    Vector3 half_extents;
    Color color;

    float sdf(Point3 p) const;
};