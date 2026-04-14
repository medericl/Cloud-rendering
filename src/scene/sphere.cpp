#include "sphere.hh"
#include "../vector/vector.hh"
#include "../image/color.hh"

Sphere::Sphere(const Point3& c, float r, const Color& col)
    : center(c), radius(r), color(col)
{ }
