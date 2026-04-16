#include "worley.hh"
#include <cmath>
#include <iostream>
#include "../vector/vector.hh"

static float hash_float(int n)
{
    n = (n << 13) ^ n;
    int m = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0f - m / 1073741824.0f;
}

static Point3 hash3(int x, int y, int z)
{
    int nx = x * 1619 + y * 31337 + z * 6271;
    int ny = x * 4231 + y * 1997 + z * 8191;
    int nz = x * 7643 + y * 5237 + z * 2381;
    return Point3(
        (hash_float(nx) + 1.0f) * 0.5f,
        (hash_float(ny) + 1.0f) * 0.5f,
        (hash_float(nz) + 1.0f) * 0.5f
    );
}

float worley(Point3 p)
{
    float h = 0;
    //float point_feature = hash3(p.x, p.y, p.z);
    float min = HUGE_VAL;
    for (int i = -1; i < 2; i += 1)
    {
    for (int j = -1; j < 2; j += 1)
    {
    for (int k = -1; k < 2; k += 1)
    {

        Point3 a = hash3(std::floor(p.x) + i, std::floor(p.y) + j, std::floor(p.z) + k);
        a.x += std::floor(p.x) + i;
        a.y += std::floor(p.y) + j;
        a.z += std::floor(p.z) + k;
        float dist = (p - a).norm();
        min = std::min(dist, min);
    }
    }
    }

    //std::cout << min << "\n";
    return min;
}