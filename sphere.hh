#ifndef SPHERE_HH
#define SPHERE_HH

#include "vector.hh"
#include "color.hh"

class Sphere {
public:
    Sphere(const Point3& c, float r, const Color& col);

    Point3 center;
    float radius;
    Color color;
};

#endif
