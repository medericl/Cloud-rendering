#ifndef CAMERA_HH
#define CAMERA_HH

#include "plan_image.hh"

class Camera {
public:
    Point3 origin;
    Point3 p;
    Vector3 direction_up;
    Plan_image plan_image;

    Camera();
    Camera(Point3 origin, Point3 p, Vector3 direction_up, float width, float height); // plan image avec la taille du plan image
    Camera(Point3 origin, Point3 p, Vector3 direction_up, float angle_x, float angle_y, bool use_angles); // plan image avec angle focale de la caméra
};

#endif
