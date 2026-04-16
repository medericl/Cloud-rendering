#include "../scene/config.hh"
#include "../scene/light.hh"
#include "../scene/sphere.hh"
#include "../scene/cube.hh"
#include "../vector/vector.hh"
#include "scene_init.hh"

#ifndef NO_UI
#endif

#include <GLFW/glfw3.h>


std::vector<Sphere> vector_sphere()
{
    std::vector<Sphere> spheres = {
        //Sphere(Point3(-200, 55, 0), 50, Color(30,130,30)), // red
        //Sphere(Point3(0, 55, 0), 51, Color(30,30,130)), // red
        //Sphere(Point3(0, 55, 0), 52, Color(130,30,30)), // red
    };
    return spheres;
}

std::vector<Cube> vector_cube()
{
    return { 
   //     Cube(Point3(0, 0, 0), Vector3(400, 12, 400), Color(30, 30, 30)),
    };
}

std::vector<Light> vector_light()
{
    std::vector<Light> lights = {
        Light(Point3(-400,600,400), COLOR_LIGHT1),
        Light(Point3(400,600,-400), COLOR_LIGHT2),

        Light(Point3(400,600,400), COLOR_LIGHT3),
        Light(Point3(-400,600,-400), COLOR_LIGHT4),


        Light(Point3(400,-60,400), Color(20,20,20)),
        Light(Point3(-400,-60,-400), Color(20,20,20)),
    };
    return lights;
}
