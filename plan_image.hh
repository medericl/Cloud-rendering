#ifndef PLAN_IMAGE_HH
#define PLAN_IMAGE_HH

#include <iostream>
#include "vector.hh"

class Plan_image {
public:
    Plan_image(const Point3& center, const Vector3& p, const Vector3& direction_up, float width, float height);

    float p_image_height;
    float p_image_width;
    Point3 p_right_up;
    Point3 p_left_up;
    Point3 p_right_down;
    Point3 p_left_down;

};

#endif

