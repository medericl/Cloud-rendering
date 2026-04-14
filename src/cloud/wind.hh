#pragma once
#include "../vector/vector.hh"

class Wind {
    public:
    Wind();
    void update();
    float last_time;
    float wind_speed;
    Vector3 wind_direction;
    Vector3 offset;
    Vector3 wind();
};