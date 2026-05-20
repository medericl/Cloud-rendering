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

float coveragemask(Point3 p)
{
    // Soft 2D weather mask: shapes large cloud groups without fully erasing density.
    float K_COVERAGE = 0.15f;
    float base = 0.55f;
    float strength = 0.45f;
    float a = (fbm(Point3(p.x * COVERAGE_FREQ, 0.0f, p.z * COVERAGE_FREQ), COVERAGE_OCT) * 0.5f) + 0.5f;
    float mask = smooth_step(K_COVERAGE, 1.0f, a);
    return base + strength * mask;
}

float worley_mix(Point3 p)
{
    //float wworley = std::max(0.0f, worley(p * 0.008f));
    //float wworley_2 = std::max(0.0f, worley(p * 0.002f));
    //float wworley_3 = std::max(0.0f, worley(p * 0.004f));
    //float res_worley = wworley_3 * 0.625f + wworley_2 * 0.25f + wworley * 0.125f;

    float w1 = std::max(0.0f, worley(p * FREQ_WORLEY));
    float w2 = std::max(0.0f, worley(p * FREQ_WORLEY * 2.0f));
    float w3 = std::max(0.0f, worley(p * FREQ_WORLEY * 4.0f));
    //float worley_mix = w1 * 0.625f + w2 * 0.25f + w3 * 0.125f;
    float worley_mix = w1 * 0.5f + w2 * 0.3f + w3 * 0.2f;

    return worley_mix;
}

float worley(Point3 p, float shape)
{
    float wworley = std::max(0.0f, worley(p * FREQ_WORLEY));
    if (W1)
        shape = wworley * K_WORLEY;
    if (WORLEY_)
        shape = shape - wworley * K_WORLEY;
    if (WORLEY_MIX)
        shape = shape - worley_mix(p) * K_WORLEY;
    return shape;
}

float cloud_density(Point3 p, Wind w)
{
    if (WIND)
        p = p + w.wind();

    float shape = fbm(p * FREQ_SHAPE, OCTAVE);
    if (VERTICAL_FALL_OFF)
        shape *= vertical_falloff(p.y);
    float detail = fbm(p * FREQ_DETAIL, OCTAVE);
    float detail_pos = std::max(0.0f, detail) * K_DETAIL;
    if (W1 || WORLEY_ || WORLEY_MIX)
        shape = worley(p, shape);
    
    float eroded = remap_smooth(shape, detail_pos, 1.0f, 0.0f, CLOUD_COVERAGE); // nuage rond

    if (COVERAGE) {
        float coverage = coveragemask(p);
        eroded -= (1.0f - coverage) * K_COVERAGE;
    }

    return std::max(0.0f, DENSITY * eroded);
}

float hg_phase(Vector3 ray, Vector3 p, float k)
{
    Vector3 sunDir = SUN_POS - p;
    float sunDistance = sunDir.norm();
    if (sunDistance <= 1e-6f)
        return 1.0f;

    Vector3 lightDir = sunDir / sunDistance;
    Vector3 viewDir = ray * -1;

    float cosTheta = lightDir.dot(viewDir);
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    float k2 = k * k;
    return (1.0f - k2) / std::pow(1.0f + k2 - 2.0f * k * cosTheta, 1.5f);
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

static float cloud_density_fast(Point3 p, Wind w)
{
    p = p + w.wind();
    float shape = fbm(p * FREQ_SHAPE, OCTAVE);
    if (VERTICAL_FALL_OFF)
        shape *= vertical_falloff(p.y);
    return std::max(0.0f, DENSITY * shape * CLOUD_COVERAGE);
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
        res += cloud_density_fast(sample, w) * step;
    }


    return std::max(0.0f, res);
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
            float jitter = (std::abs(std::fmod(std::sin(ray.x * 12.9898f + ray.y * 78.233f) * 43758.5453f, 1.0f))) * 1.5f;
            ti = tStart + (i + jitter) * step;
        }
        Point3 p = (origin + ray * ti);

        if (FOG) {
               
        }

        float density = cloud_density(p, w);


        if (density <= 0.0001f) {
            continue;
        }

        float volume = transmittance * density * step;
        if (!LIGHT)
        {
            accum += fog_color * K_AMBIENT_CLOUD * volume;
        }
        else
        {
            float sun_transmittance = beer_powder(sun_density(p,w)); // cmb les nuage bloque la lumiere
            Color light = (fog_color * COLOR_SUN) / 255.0f * sun_transmittance;
            if (HG)
            {
                float hg_soft = hg_phase(ray, p, 0.45);
                float hg_silver = hg_phase(ray, p, 0.85);
                float phase = hg_soft + hg_silver + K_HG;
                accum += light * volume * phase; // reflets
            }
            else
                accum += light * volume;
        }

        if (AMBIENT_CLOUD)
            accum += CLOUD_SHADOW_COLOR * K_AMBIENT_CLOUD * volume;

        transmittance *= std::exp(-density * step);
        if (transmittance <= 0.001f)
            break;
    }

    return pixel * transmittance + accum;
}
