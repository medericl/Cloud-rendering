#include "plan_image.hh"

Plan_image::Plan_image(const Point3& center, const Vector3& p, const Vector3& direction_up, float width, float height)
{
    p_image_height = height;
    p_image_width = width;

    Vector3 direction_plan = p / p.norm();
    Vector3 right = direction_up.p_v(direction_plan) / direction_up.p_v(direction_plan).norm();
    Vector3 up = direction_plan.p_v(right) / direction_plan.p_v(right).norm();

    Vector3 v_plan_image_up = up * (p_image_height/ 2);
    Vector3 v_plan_image_right = right * (p_image_width / 2);

    p_right_up   = center + v_plan_image_up + v_plan_image_right;
    p_left_up    = center + v_plan_image_up - v_plan_image_right;
    p_right_down = center - v_plan_image_up + v_plan_image_right;
    p_left_down  = center - v_plan_image_up - v_plan_image_right;
}