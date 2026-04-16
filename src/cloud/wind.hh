#pragma once
#include "../vector/vector.hh"

class Wind {
    public:
    Wind();
    void update();
    float last_time;
    Vector3 offset;
    Vector3 wind();
};