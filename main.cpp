#include "run_window.hh"
#include "src/scene/camera.hh"

int main() {
    //int width = 1920;
    //int height = 1080;
    int width = 1000;
    int height = 1000;

    Camera camera(Point3(800,500,-800), Point3(180,180,-180), Vector3(0,1,0), width, height);
    run_window(width, height, camera);
    return 0;
}
