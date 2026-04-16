#include "scene.hh"
#include "../image/color.hh"
#include "sphere.hh"
#include "../vector/vector.hh"
#include "config.hh"
#include "../cloud/effect.hh"
#include "../cloud/fmb.hh"

#include <algorithm>
#include <cmath>
#include <vector>

Scene::Scene(Camera Camera, std::vector<Light> List_light, std::vector<Sphere> List_sphere,
             std::vector<Cube> List_cube)
: camera(Camera), list_light(List_light), list_sphere(List_sphere), list_cube(List_cube)
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
    for (auto& c : list_cube)
        box_d = std::min(box_d, c.sdf(p));
    return std::min(spheres_d, box_d);
}

float Scene::sdf(Vector3 p) {
    float d = HUGE_VAL;
    for (auto& s : list_sphere)
        d = std::min(d, (p - s.center).norm() - s.radius);
    for (auto& c : list_cube)
        d = std::min(d, c.sdf(p));
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

Color Scene::find_color(Point3 inter_point)
{
    // Check if we hit a cube (box)
    float cube_dist = HUGE_VAL;
    for (auto& c : list_cube)
        cube_dist = std::min(cube_dist, c.sdf(inter_point));
    float sphere_dist = HUGE_VAL;
    for (auto& s : list_sphere)
        sphere_dist = std::min(sphere_dist, (inter_point - s.center).norm() - s.radius);

    if (cube_dist < sphere_dist + 0.1f)
        return COLOR_FLOOR;

    float total_weight = 0.0f;
    Color result(0, 0, 0);

    for (auto& sphere : list_sphere) {
        float dist = (inter_point - sphere.center).norm();
        float weight = 1.0f / (dist * dist);
        result = result + sphere.color * weight;
        total_weight += weight;
    }

    return result * (1.0f / total_weight);
}


Color Scene::shade(Point3 origin, Vector3 ray, float t)
{
    Point3 P = origin + ray * t; // intersection point
    Vector3 N = normal(P);
    Vector3 R = (P - origin) / (P - origin).norm(); // ray
    Vector3 S = R - N * (R.dot(N)) * 2; // rayon réfléchi
    Color res;
    for (auto& light : list_light)
    {
        Vector3 L = (light.origin - P) / (light.origin - P).norm(); // source lumineuse
        Color color_object = find_color(P);

        Color diffuse = color_object * std::max(0.0f, L.dot(N)) * 0.4;
        Color specular = light.color * std::pow(std::max(0.0f, S.dot(L)), ns) * ks;
        // Color reflected = ray_march(S, P, n - 1) * kr;
        res = res + diffuse + specular;
    }

    return res + ambient_color;
}

float Scene::ray_march(Vector3 ray, Point3 origin)
{
    size_t ray_iteration = 90;
    float t = 0;
    for (size_t i = 0; i < ray_iteration; i += 1) {
        Point3 p = origin + ray * t;

        float d = sdf_smooth(p);
        if (d < k_close) {
            return t;
        }

        if (d > k_far) {
            return 0;
        }

        t += d;
    }

    return 0;
}

bool intersect_box(const Point3& origin, const Vector3& ray, const Point3& box_min, const Point3& box_max, float& tEnter, float& tExit)
{
    Vector3 invRd(1.0f / ray.x, 1.0f / ray.y, 1.0f / ray.z);

    // Point3 p = origin + ray * t;
    Vector3 t0 = (box_min - origin) * invRd; // t0 = (entrée - origin) / ray
    Vector3 t1 = (box_max - origin) * invRd; // t1 = (sortie - origin) / ray

    Vector3 tsmall(std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z)); // probleme de signe
    Vector3 tbig(std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z)); // probleme de signe si t0 est négatif

    tEnter = std::max(std::max(tsmall.x, tsmall.y), tsmall.z); // intersection entrée
    tExit  = std::min(std::min(tbig.x, tbig.y), tbig.z); // intersection sortie

    return tExit > std::max(tEnter, 0.0f);
}

Color Scene::IntegrateVolume(Point3 origin, Vector3 ray, Color pixel, float t)
{
    Point3 boxMin(BOX_MIN_X, BOX_MIN_Y, BOX_MIN_Z);
    Point3 boxMax(BOX_MAX_X, BOX_MAX_Y, BOX_MAX_Z);

    float tEnter, tExit;
    if (!intersect_box(origin, ray, boxMin, boxMax, tEnter, tExit))
        return pixel;

    float tStart = std::max(tEnter, 0.0f);
    float tEnd = tExit;

    if (t > 0.0f)
        tEnd = std::min(tExit, t);

    if (tStart >= tEnd) {
        return pixel;
    }

    float accum = 0.0f;
    float transmittance = 1.0f;
    float step = (tEnd - tStart) / FOG_STEPS;
    for (int i = 0; i < FOG_STEPS; i++)
    {
        float ti = tStart + i * step;
        Point3 p = (origin + ray * ti) + w.wind();
        float shape = vertical_falloff(p.y) * fbm(p, 4);
        float detail = fbm(p * 4.0f, 4);
        //float erosion = worley(shape);
        float detail_pos = std::max(0.0f, detail) * 0.6f;
        float eroded = remap(shape, detail_pos, 1.0f, 0.0f, 1.2f);

        float density = std::max(0.0f, DENSITY * eroded);
        
        accum += transmittance * density * step;
        transmittance *= std::exp(-density * step);
        if (transmittance <= 0.001f)
            break;
    }

    return pixel * transmittance + fog_color * accum;
}

void Scene::ray_tracing(Image& image)
{
    w.update();
    // interpole tous les points du plan_image
    // trouve les vecteurs qui partent de l'origine
    Plan_image p_i = camera.plan_image;

    int height = p_i.p_image_height;
    int width = p_i.p_image_width;
    std::vector<Color> canvas(height * width);

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

            float dist = ray_march(ray, camera.origin);
            if (dist != 0) // touched an object
                pixel = shade(camera.origin, ray, dist);

            Color finalColor = IntegrateVolume(camera.origin, ray, pixel, dist);
            image.setPixel(x, y, finalColor);
        }
    }
}
