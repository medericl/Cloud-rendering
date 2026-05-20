#include "terrain.hh"
#include "config.hh"
#include <cmath>
#include <algorithm>

struct Layer { float kx, kz, amp; };
static const Layer LAYERS[] = {
    { 0.009f,  0.003f, 110.0f},
    { 0.005f,  0.029f,  55.0f},
    { 0.068f, -0.032f,  20.0f},
    {-0.120f,  0.145f,   8.0f},
    { 0.160f, -0.440f,   3.0f},
};
static const int NL = 5;

float terrain_height(float x, float z)
{
    float h = 0;
    for (int i = 0; i < NL; i++)
        h += LAYERS[i].amp * std::sin(x * LAYERS[i].kx + z * LAYERS[i].kz);
    return h * TERRAIN_SCALE - 30.0f;
}

static Vector3 terrain_normal(float x, float z)
{
    float dnx = 0, dnz = 0;
    for (int i = 0; i < NL; i++) {
        const Layer& l = LAYERS[i];
        float c = std::cos(x * l.kx + z * l.kz);
        dnx += l.amp * l.kx * c;
        dnz += l.amp * l.kz * c;
    }
    Vector3 n(-dnx * TERRAIN_SCALE, 1.0f, -dnz * TERRAIN_SCALE);
    return n / n.norm();
}

static float cheap_tex(float x, float z)
{
    float a = std::sin( x * 1.7f + z * 0.9f);
    float b = std::sin( x * 0.5f - z * 1.9f);
    float c = std::sin(-x * 1.2f + z * 1.3f);
    return (a + b + c) * (1.0f / 3.0f);
}

static Color shade_terrain(Point3 p, float dist)
{
    Vector3 normal = terrain_normal(p.x, p.z);
    float slope = normal.y;

    float h_min = -150.0f, h_max = 140.0f;
    float height_frac = (p.y - h_min) / (h_max - h_min);
    height_frac = std::max(0.0f, std::min(1.0f, height_frac));

    float lod = std::max(0.0f, std::min(1.0f, (1200.0f - dist) / 800.0f));

    float frac = height_frac;
    if (lod > 0.01f) {
        float wx = cheap_tex(p.x * 0.08f,        p.z * 0.08f)        * 4.0f * lod;
        float wz = cheap_tex(p.x * 0.08f + 1.5f, p.z * 0.08f + 0.8f) * 4.0f * lod;
        float tx = p.x + wx, tz = p.z + wz;
        float detail = cheap_tex(tx * 0.25f,        tz * 0.25f)        * 0.13f
                     + cheap_tex(tx * 0.80f + 1.1f, tz * 0.80f + 2.2f) * 0.05f;
        frac += detail * lod - 0.09f * lod;
    }

    frac += (TERRAIN_TEXTURE - 0.5f) * 0.8f;
    frac = std::max(0.0f, std::min(1.0f, frac));

    Color ground(45, 38, 28);
    Color scrub(58, 65, 38);
    Color rock(80, 74, 63);
    Color snow(210, 215, 225);

    Color height_col;
    if (frac < 0.30f) {
        float f = frac / 0.30f;
        height_col = ground * (1.0f - f) + scrub * f;
    } else if (frac < 0.75f) {
        float f = (frac - 0.30f) / 0.45f;
        height_col = scrub * (1.0f - f) + rock * f;
    } else {
        float f = (frac - 0.75f) / 0.25f;
        height_col = rock * (1.0f - f) + snow * f;
    }

    Color cliff(58, 52, 46);
    if (lod > 0.01f) {
        float strata = 0.5f + 0.5f * std::sin(p.y * 0.12f
                     + cheap_tex(p.x * 0.05f, p.z * 0.05f) * 2.0f);
        cliff = cliff * (0.8f + strata * 0.2f * lod + 0.2f * (1.0f - lod));
    }

    float steepness = std::max(0.0f, 1.0f - slope * 2.5f);
    Color col = height_col * (1.0f - steepness) + cliff * steepness;

    Vector3 sun = SUNSET ? Vector3(1.0f, 0.15f, 0.0f) : Vector3(0.4f, 1.0f, 0.2f);
    sun = sun / sun.norm();
    float diffuse = std::max(0.25f, normal.dot(sun));

    return col * diffuse;
}

bool terrain_hit(Point3 origin, Vector3 ray, float& t_hit, Color& col)
{
    float t = 5.0f, prev_t = t, t_max = 4000.0f;

    while (t < t_max) {
        Point3 p = origin + ray * t;

        if (p.x < BOX_MIN_X || p.x > BOX_MAX_X || p.z < BOX_MIN_Z || p.z > BOX_MAX_Z) {
            prev_t = t;
            t += 50.0f;
            continue;
        }

        float above = p.y - terrain_height(p.x, p.z);
        if (above < 0.0f) {
            float lo = prev_t, hi = t;
            for (int i = 0; i < 8; i++) {
                float mid = (lo + hi) * 0.5f;
                Point3 pm = origin + ray * mid;
                if (pm.y < terrain_height(pm.x, pm.z)) hi = mid;
                else                                    lo = mid;
            }
            t_hit = lo;
            col = shade_terrain(origin + ray * t_hit, t_hit);
            return true;
        }

        prev_t = t;
        t += std::max(3.0f, above * 0.3f);
    }
    return false;
}
