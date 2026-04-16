#pragma once
#include <vector>
#include "src/scene/scene.hh"
#include "src/scene/camera.hh"

void render_to_pixels(int width, int height, float x_offset, std::vector<unsigned char>& pixels, Scene& scene);
void run_window(int width, int height, Camera cam);
