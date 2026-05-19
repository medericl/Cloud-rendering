#include "../scene/config.hh"
#include "../scene/light.hh"
#include "../scene/sphere.hh"
#include "../vector/vector.hh"
#include "scene_init.hh"

#ifndef NO_UI
#endif

#include <GLFW/glfw3.h>


std::vector<Sphere> vector_sphere()
{
    std::vector<Sphere> spheres = { Sphere(Point3(-200, 55, 0), 50, Color(30,130,30)), // red
    };
    return spheres;
}

std::vector<Light> vector_light()
{
    std::vector<Light> lights = { Light(SUN_POS, COLOR_SUN), };
    return lights;
}
