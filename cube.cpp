#include "cube.hh"
#include "vector.hh"
#include "color.hh"
#include <cmath>
#include <algorithm>

Cube::Cube(const Point3& center, const Vector3& half_extents, const Color& color)
    : center(center), half_extents(half_extents), color(color)
{ }

// SDF d'une boîte axis-aligned
float Cube::sdf(Point3 p) const {
    Vector3 d(
        std::abs(p.x - center.x) - half_extents.x,
        std::abs(p.y - center.y) - half_extents.y,
        std::abs(p.z - center.z) - half_extents.z
    );
    Vector3 d_pos(
        std::max(d.x, 0.0f),
        std::max(d.y, 0.0f),
        std::max(d.z, 0.0f)
    );
    float outside = d_pos.norm();
    float inside  = std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);
    return outside + inside;
}
