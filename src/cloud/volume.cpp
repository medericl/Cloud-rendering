#include "../image/color.hh"
#include "../vector/vector.hh"
#include "../scene/config.hh"
#include "../cloud/fall_off.hh"
#include "../cloud/fbm.hh"
#include "wind.hh"
#include "worley.hh"

#include <algorithm>
#include <cmath>
#include <iostream>


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

    float accum = 0.0f;
    float transmittance = 1.0f;
    float step = (tEnd - tStart) / FOG_STEPS;
    for (int i = 0; i < FOG_STEPS; i++)
    {
        float ti = tStart + i * step;
        Point3 p = (origin + ray * ti) + w.wind();

        float shape = vertical_falloff(p.y) * fbm(p * FREQ_SHAPE, OCTAVE);
        //float detail = fbm(p * FREQ_DETAIL, OCTAVE) * (1 - worley(p * K_WORLEY));
        float detail = fbm(p * FREQ_DETAIL, OCTAVE);
        //float erosion = worley(shape);


        float Worley = pow(1 - worley(p * K_WORLEY),3);
        float detail_pos = std::max(0.0f, detail) * K_DETAIL;
        //detail_pos = std::max(0.0f,worley(p * K_WORLEY) * K_DETAIL);

        float eroded = remap_smooth(shape, detail_pos, 1.0f, 0.0f, CLOUD_COVERAGE) - (Worley * 3.55f);
        //float eroded = remap_smooth(shape, detail_pos, 1.0f, 0.0f, CLOUD_COVERAGE);


        float density = std::max(0.0f, DENSITY * eroded);
        
        accum += transmittance * density * step;
        transmittance *= std::exp(-density * step);
        if (transmittance <= 0.001f)
            break;
    }

    return pixel * transmittance + fog_color * accum;
}
