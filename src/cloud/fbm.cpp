#include "../scene/config.hh"
#include "fbm.hh"
#include "fall_off.hh"

#include <cmath>

static float hash3(int x, int y, int z)
{
    int n = x * 1619 + y * 31337 + z * 6271;
    n = (n << 13) ^ n;
    int m = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0f - m / 1073741824.0f;
    //return m / 2147483647.0f;
}

static float fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

static float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float noise(Point3 p)
{
    int ix = (int)std::floor(p.x);
    int iy = (int)std::floor(p.y);
    int iz = (int)std::floor(p.z);

    float fx = p.x - ix;
    float fy = p.y - iy;
    float fz = p.z - iz;

    //float ux = fade(fx);
    //float uy = fade(fy);
    //float uz = fade(fz);

    float ux = smooth_step(0.0f, 1.0f, fx);
    float uy = smooth_step(0.0f, 1.0f, fy);
    float uz = smooth_step(0.0f, 1.0f, fz);

    float v000 = hash3(ix,iy,iz);
    float v100 = hash3(ix+1, iy,iz);
    float v010 = hash3(ix,iy+1,iz);
    float v110 = hash3(ix+1,iy+1,iz);
    float v001 = hash3(ix,iy,iz+1);
    float v101 = hash3(ix+1,iy,iz+1);
    float v011 = hash3(ix,iy+1,iz+1);
    float v111 = hash3(ix+1,iy+1,iz+1);

    float a = lerp(v000, v100, ux);
    float b = lerp(v010, v110, ux);
    float a1 = lerp(v001, v101, ux);
    float a2 = lerp(v011, v111, ux);

    return lerp( lerp(a, b, uy), lerp(a1, a2, uy), uz);
}

float remap_smooth(float val, float low, float high, float min, float max)
{
    float t = (val - low) / (high - low); // pourcentage entre low and high
    t = std::max(0.0f, std::min(1.0f, t)); // normalise entre 0 et 1
    return min + t * (max - min); // changer l'intervale
}

float remap_classic(float val)
{
    float low = 0.3f;
    float high = 0.7f;
    float t = (val - low) / (high - low);
    t = std::max(0.0f, std::min(1.0f, t));
    return t * t * (3.0f - 2.0f * t);
}

float remap_sharp(float val)
{
    float k = 1.3f;
    return std::pow(val, k);
}

float remap_soft(float val)
{
    return val * val;
}

float fbm(Point3 p, int octave)
{
    float sum = 0;
    float amplitude = INITIAL_AMPLITUDE;
    float frequency = INITIAL_FREQUENCY;

    for (int i = 0; i < octave; i+=1)
    {
        float value = noise(p * frequency);
        sum += amplitude * value;
        //frequency *= 1.8; // 2
        //amplitude *= 0.5;
        frequency *= LACUNARITY;
        amplitude *= GAIN;

    }

    return sum;
    //return remap_classic(sum);
}