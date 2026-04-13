#include "sphere.hh"
#include "vector.hh"
#include "color.hh"

Sphere::Sphere(const Point3& c, float r, const Color& col)
    : center(c), radius(r), color(col)
{ }
