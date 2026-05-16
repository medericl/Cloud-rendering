#include "scene.hh"
#include "../image/color.hh"
#include "sphere.hh"
#include "../vector/vector.hh"
#include "../cloud/volume.hh"
#include "config.hh"

#include <algorithm>
#include <cmath>
#include <vector>

Scene::Scene(Camera Camera, std::vector<Light> List_light, std::vector<Sphere> List_sphere,
             std::vector<Cube> List_cube)
: camera(Camera), list_light(List_light), list_sphere(List_sphere), floor(Cube(Point3(0, 0, 0), Vector3(400, 12, 400), Color(30, 30, 30)))
{ };

float Scene::sdf_diff(Vector3 p) {
    float d = HUGE_VAL;
    float ma = HUGE_VAL;

    for (auto& s : list_sphere)
    {
        if (s.color.g == 31) {
            float buff = (p - s.center).norm() - s.radius;
            ma = std::min(ma, buff);
        }
        else {
            float buff = (p - s.center).norm() - s.radius;
            d = std::min(d, buff);
        }
    }

    return std::max(d, -ma);
}

float Scene::sdf_smooth(Vector3 p) {
    float d = HUGE_VAL;
    float d1 = HUGE_VAL;
    for (auto& s : list_sphere)
    {
        float new_d = (p - s.center).norm() - s.radius;
        if (d > new_d) {
            d1 = d;
            d = new_d;
        }
        else if (new_d < d1) {
            d1 = new_d;
        }
    }

    float k = 100;
    float spheres_d;
    if (std::abs(d - d1) < k)
    {
        float h = std::max((k - std::abs(d-d1)) / k, 0.0f);
        spheres_d = d - (h*h*h) * k / 6.0f;
    }
    else
    {
        spheres_d = d;
    }

    float box_d = HUGE_VAL;

    //box_d = std::min(box_d, floor.sdf(p));
    return std::min(spheres_d, box_d);
}

float Scene::sdf(Vector3 p) {
    float d = HUGE_VAL;
    for (auto& s : list_sphere)
        d = std::min(d, (p - s.center).norm() - s.radius);
    //d = std::min(d, floor.sdf(p));
    return d;
}


Vector3 Scene::normal(Point3 P) {
    float eps = 0.01;
    float dx = sdf(P + Vector3(eps, 0, 0)) - sdf(P - Vector3(eps, 0, 0));
    float dy = sdf(P + Vector3(0, eps, 0)) - sdf(P - Vector3(0, eps, 0));
    float dz = sdf(P + Vector3(0, 0, eps)) - sdf(P - Vector3(0, 0, eps));
    Vector3 grad(dx, dy, dz);
    return grad / grad.norm();
}

void Scene::ray_tracing(Image& image)
{
    wind.update();
    // interpole tous les points du plan_image
    // trouve les vecteurs qui partent de l'origine
    Plan_image p_i = camera.plan_image;

    int height = image.height;
    int width = image.width;

    Point3 left = p_i.p_left_up;
    Point3 right = p_i.p_right_up;
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; y++) {
        double t_y = y / (double)(height - 1);
        Point3 left_row = left + (p_i.p_left_down - left) * t_y;
        Point3 right_row = right + (p_i.p_right_down - right) * t_y;

        for (int x = 0; x < width; x++) {
            double t_x = x / (double)(width - 1);
            Point3 point = left_row + (right_row - left_row) * t_x;
            Vector3 ray = (point - camera.origin) / (point - camera.origin).norm();
            Color pixel = background_color;

            Color finalColor = IntegrateVolume(camera.origin, ray, pixel, 0.0f, wind);
            image.setPixel(x, y, finalColor);
        }
    }
}
