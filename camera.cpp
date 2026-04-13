#include "camera.hh"
#include "plan_image.hh"
#include <cmath>

Camera::Camera() 
    : origin(100,100,0), p(0,0, 700), direction_up(0,1,0), plan_image(p, p - origin, direction_up, 1400, 1000)
{ }

Camera::Camera(Point3 origin, Point3 p, Vector3 direction_up, float width, float height)
    : origin(origin), p(p), direction_up(direction_up), plan_image(p, p - origin, direction_up, width, height)
{ }

Camera::Camera(Point3 origin, Point3 p, Vector3 direction_up, float angle_x, float angle_y, bool)
    : origin(origin), p(p), direction_up(direction_up), plan_image(p, p - origin, direction_up,
        // angle en degré
                 2.0f * (p - origin).norm() * std::tan((angle_x  * M_PI / 180.0f) / 2.0f),
                 2.0f * (p - origin).norm() * std::tan((angle_y * M_PI / 180.0f) / 2.0f))
{ }
