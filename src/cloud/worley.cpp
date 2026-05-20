#include "worley.hh"
#include <cmath>
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
    int ix = (int)std::floor(p.x);
    int iy = (int)std::floor(p.y);
    int iz = (int)std::floor(p.z);
    float min_sq = HUGE_VAL;

    for (int i = -1; i < 2; i++)
    for (int j = -1; j < 2; j++)
    for (int k = -1; k < 2; k++)
    {
        Point3 a = hash3(ix + i, iy + j, iz + k);
        a.x += ix + i;
        a.y += iy + j;
        a.z += iz + k;
        float dx = p.x - a.x;
        float dy = p.y - a.y;
        float dz = p.z - a.z;
        float dist_sq = dx*dx + dy*dy + dz*dz;
        if (dist_sq < min_sq) min_sq = dist_sq;
    }
    return std::sqrt(min_sq);
}