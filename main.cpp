#include "run_window.hh"
#include "src/scene/camera.hh"

int main() {
    int width = 300;
    int height = 300;
    Camera camera(Point3(800,500,-800), Point3(661,426,-661), Vector3(0,1,0), width, height);
    run_window(width, height, camera);
    return 0;
}
