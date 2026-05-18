#include "../image/color.hh"
#include "../vector/vector.hh"
#include "../scene/config.hh"
#include "../cloud/fall_off.hh"
#include "../cloud/fbm.hh"
#include "wind.hh"
#include "worley.hh"

#include <algorithm>
#include <cmath>


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

float cloud_density(Point3 p, Wind w)
{
    p = p + w.wind();
    float shape = fbm(p * FREQ_SHAPE, OCTAVE);
    if (VERTICAL_FALL_OFF)
        shape *= vertical_falloff(p.y);

    float detail = fbm(p * FREQ_DETAIL, OCTAVE);
    //float Worley = pow(1 - worley(p * FREQ_WORLEY),3);
    float wworley = std::max(0.0f, worley(p * FREQ_WORLEY));

    float detail_pos = std::max(0.0f, detail) * K_DETAIL;

    //float eroded = remap_smooth(shape, detail_pos, 1.0f, 0.0f, CLOUD_COVERAGE) - (Worley * 3.55f);
    float eroded = remap_smooth(shape, detail_pos, 1.0f, 0.0f, CLOUD_COVERAGE) - wworley * K_WORLEY; // nuage rond

    return std::max(0.0f, DENSITY * eroded);
}

float hg_phase(Vector3 ray, Vector3 p)
{
    Vector3 sunDir = SUN_POS - p;
    float sunDistance = sunDir.norm();
    if (sunDistance <= 1e-6f)
        return 1.0f;

    Vector3 lightDir = sunDir / sunDistance;
    Vector3 viewDir = ray * -1;

    float cosTheta = lightDir.dot(viewDir);
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    float g2 = G_HG * G_HG;
    return (1.0f - g2) / std::pow(1.0f + g2 - 2.0f * G_HG * cosTheta, 1.5f);
}

float beer_powder(float lightDepth)
{
    float beer = std::exp(-lightDepth);
    float powder = 0.5f + 0.5f * (1.0f - std::exp(-lightDepth * POWDER_STRENGTH));
    float amount = std::max(0.0f, std::min(1.0f, POWDER_AMOUNT));
    float beerPowder = beer * ((1.0f - amount) + amount * powder);
    //float beerPowder = beer * (1.0f + powder * POWDER_AMOUNT);
    return std::max(0.0f, std::min(1.0f, beerPowder));
}

float sun_density(Point3 p, Wind w)
{
    Vector3 sun_ray = SUN_POS - p;
    float sun_distance = sun_ray.norm();
    if (sun_distance <= 0.0f || SUN_STEPS <= 0)
        return 0.0f;

    sun_ray = sun_ray / sun_distance;

    Point3 boxMin(BOX_MIN_X, BOX_MIN_Y, BOX_MIN_Z);
    Point3 boxMax(BOX_MAX_X, BOX_MAX_Y, BOX_MAX_Z);
    float tEnter, tExit;
    if (!intersect_box(p, sun_ray, boxMin, boxMax, tEnter, tExit))
        return 0.0f;

    float sun_exit_distance = std::max(0.0f, tExit);
    if (sun_exit_distance <= 0.0f)
        return 0.0f;

    float res = 0.0f;
    float step = sun_exit_distance / SUN_STEPS;
    for (int i = 0; i < SUN_STEPS; i++)
    {
        Point3 sample = p + sun_ray * (i * step);
        res += cloud_density(sample, w) * step;
    }
    return std::max(0.0f, res);
}

float coveragemask(Point3 p)
{
    // 2D cloud coverage mask, controls where clouds appear on the XZ space.
    return (fbm(Point3(p.x * COVERAGE_FREQ, 0.0f, p.z * COVERAGE_FREQ), COVERAGE_OCT) * 0.5f) + 0.5f;
}

Color IntegrateVolume(Point3 origin, Vector3 ray, Color pixel, float t, Wind w)
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

    Color accum = Color(0,0,0);
    float transmittance = 1.0f;
    float step = (tEnd - tStart) / FOG_STEPS;
    for (int i = 0; i < FOG_STEPS; i++)
    {
        float ti = tStart + i * step;
        if (JITTER) {
            float jitter = (std::abs(std::fmod(std::sin(ray.x * 12.9898f + ray.y * 78.233f) * 43758.5453f, 1.0f))) * 3.0f;
            ti = tStart + (i + jitter) * step;
        }
        Point3 p = (origin + ray * ti);

        float density = cloud_density(p, w);

        if (COVERAGE) {
            density *= coveragemask(p);
        }

        if (density <= 0.0001f) {
            continue;
        }

        float sun_transmittance = beer_powder(sun_density(p,w)); // cmb les nuage bloque la lumiere
        float hg = hg_phase(ray, p); // reflets

        //accum += transmittance * sun_transmittance * density * step * hg;
        Color incomingLight = SUN * sun_transmittance;
        float light = transmittance * density * step;
        if (HG)
            accum += (fog_color * incomingLight) / 255.0f * light * hg;
        else
            accum += (fog_color * incomingLight) / 255.0f * light;

        if (AMBIENT_CLOUD)
            accum += fog_color * AMBIENT_CLOUD_COLOR * light;

        transmittance *= std::exp(-density * step);
        if (transmittance <= 0.001f)
            break;
    }

    return pixel * transmittance + accum;
}
