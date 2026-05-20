#include "terrain.hh"
#include "config.hh"
#include "../cloud/fbm.hh"
#include <cmath>
#include <algorithm>

float terrain_height(float x, float z)
{
    // large smooth base shapes
    float h = noise(Point3(x * 0.005f, 0.5f, z * 0.005f)) * 140.0f
            + noise(Point3(x * 0.018f, 0.5f, z * 0.018f) + Point3(1.7f, 0.0f, 0.3f)) * 55.0f;

    // ridged noise for rocky peaks/ridges: (1 - |n|) creates V-shaped ridges instead of bumps
    float n1 = noise(Point3(x * 0.07f,  0.5f, z * 0.07f)  + Point3(0.5f, 0.0f, 2.1f));
    float n2 = noise(Point3(x * 0.22f,  0.5f, z * 0.22f)  + Point3(3.0f, 0.0f, 1.0f));
    float n3 = noise(Point3(x * 0.65f,  0.5f, z * 0.65f)  + Point3(5.2f, 0.0f, 0.8f));
    h += (1.0f - std::abs(n1)) * 28.0f;
    h += (1.0f - std::abs(n2)) * 12.0f;
    h += (1.0f - std::abs(n3)) * 5.0f;

    // fine surface roughness (abs noise = jagged small bumps)
    h += std::abs(noise(Point3(x * 1.8f + 1.0f, 0.5f, z * 1.8f + 3.0f))) * 3.0f;

    return h * TERRAIN_SCALE - 30.0f;
}

static Color shade_terrain(Point3 p)
{
    // tighter epsilon = normal captures fine rock detail
    float eps = 0.8f;
    float dnx = (terrain_height(p.x + eps, p.z) - terrain_height(p.x - eps, p.z)) / (2.0f * eps);
    float dnz = (terrain_height(p.x, p.z + eps) - terrain_height(p.x, p.z - eps)) / (2.0f * eps);
    Vector3 normal(-dnx, 1.0f, -dnz);
    normal = normal / normal.norm();

    float slope = normal.y; // 1=flat, 0=vertical

    float h_min = -150.0f, h_max = 140.0f;
    float height_frac = (p.y - h_min) / (h_max - h_min);
    height_frac = std::max(0.0f, std::min(1.0f, height_frac));

    // domain-warped coordinates for organic texture edges
    float wx = noise(Point3(p.x * 0.018f,        0.3f, p.z * 0.018f))         * 6.0f;
    float wz = noise(Point3(p.x * 0.018f + 3.1f, 0.3f, p.z * 0.018f + 1.7f)) * 6.0f;
    float tx = p.x + wx, tz = p.z + wz;

    // multi-scale color noise
    float detail = noise(Point3(tx * 0.35f, 0.5f, tz * 0.35f)) * 0.16f
                 + noise(Point3(tx * 1.2f,  0.5f, tz * 1.2f))  * 0.06f
                 + noise(Point3(tx * 4.0f,  0.5f, tz * 4.0f))  * 0.02f;

    float frac = height_frac + detail - 0.10f;
    frac += (TERRAIN_TEXTURE - 0.5f) * 0.8f;
    frac = std::max(0.0f, std::min(1.0f, frac));

    // rocky mountain palette: muted, less green
    Color ground(45, 38, 28);
    Color scrub(58, 65, 38);
    Color rock(80, 74, 63);
    Color snow(210, 215, 225);

    Color height_col;
    if (frac < 0.3f) {
        float f = frac / 0.3f;
        height_col = ground * (1.0f - f) + scrub * f;
    } else if (frac < 0.75f) {
        float f = (frac - 0.3f) / 0.45f;
        height_col = scrub * (1.0f - f) + rock * f;
    } else {
        float f = (frac - 0.75f) / 0.25f;
        height_col = rock * (1.0f - f) + snow * f;
    }

    // steep cliff faces: add geological strata banding
    float strata = 0.5f + 0.5f * std::sin(p.y * 0.12f
                 + noise(Point3(p.x * 0.04f, 0.0f, p.z * 0.04f)) * 3.0f);
    Color cliff(58, 52, 46);
    cliff = cliff * (0.8f + strata * 0.2f);

    float steepness = std::max(0.0f, 1.0f - slope * 2.5f);
    Color col = height_col * (1.0f - steepness) + cliff * steepness;

    // directional lighting
    Vector3 sun(0.4f, 1.0f, 0.2f);
    sun = sun / sun.norm();
    float diffuse = std::max(0.25f, normal.dot(sun));

    return col * diffuse;
}

bool terrain_hit(Point3 origin, Vector3 ray, float& t_hit, Color& col)
{
    float t = 5.0f;
    float prev_t = t;
    float t_max = 4000.0f;

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
                if (pm.y < terrain_height(pm.x, pm.z))
                    hi = mid;
                else
                    lo = mid;
            }
            t_hit = lo;
            col = shade_terrain(origin + ray * t_hit);
            return true;
        }

        prev_t = t;
        t += std::max(3.0f, above * 0.3f);
    }
    return false;
}
