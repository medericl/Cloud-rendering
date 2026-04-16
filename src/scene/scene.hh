#ifndef SCENE_HH
#define SCENE_HH

#include "camera.hh"
#include "light.hh"
#include "../image/image.hh"
#include "sphere.hh"
#include "cube.hh"
#include "../vector/vector.hh"
#include "../cloud/wind.hh"

class Scene {
public:

    Camera camera;
    Cube floor;
    std::vector<Light> list_light;
    std::vector<Sphere> list_sphere;
    Wind wind;

    Scene();
    Scene(Camera Camera, std::vector<Light> list_light, std::vector<Sphere> list_sphere, std::vector<Cube> list_cube = {});
    void ray_tracing(Image& image);
    float ray_march(Vector3 ray, Point3 origin);

    float sdf_smooth(Vector3 p);
    float sdf(Vector3 p);
    float sdf_diff(Vector3 p);
    Color shade(Point3 origin, Vector3 ray, float t);
    Vector3 normal(Point3 P);
    Color find_color(Point3 inter_point);
};

#endif
