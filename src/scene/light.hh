#ifndef LIGHT_HH 
#define LIGHT_HH

#include "../vector/vector.hh"
#include "../image/color.hh"
#include "config.hh"

class Light {
public:
    Light() : origin(0,100,0), color(COLOR_LIGHT1)
    { }

    Light(Point3 o)
        : origin(o), color(COLOR_LIGHT1)
    { }

    Light(Point3 o, Color c)
        : origin(o), color(c)
    { }
    
    Point3 origin;
    Color color;
};

#endif
