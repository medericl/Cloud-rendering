#include "wind.hh"
#include <GLFW/glfw3.h>
#include "../scene/config.hh"

Wind::Wind()
{
    last_time = 0.0f;
    offset = Vector3(0.0f, 0.0f, 0.0f);
}

void Wind::update()
{
    float now = (float)glfwGetTime();
    if (last_time == 0.0f) last_time = now;
    float elapsed = now - last_time;
    last_time = now;
    if (elapsed > 0.05f) elapsed = 0.05f;
    offset = offset + WIND_DIRECTION * WIND_SPEED * elapsed;
}

Vector3 Wind::wind()
{
    return offset;
}