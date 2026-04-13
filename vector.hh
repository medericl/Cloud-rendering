#ifndef VECTOR_HH
#define VECTOR_HH

#include <ostream>

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vector3 operator+(const Vector3& v) const;

    Vector3 operator+(float t) const;

    Vector3 operator-(const Vector3& v) const;

    Vector3 operator*(float t) const;

    Vector3 operator/(float t) const;

    // produit scalaire
    float dot(const Vector3& v) const;

    // length
    float norm() const;

    // produit vectoriel
    Vector3 p_v(const Vector3& v) const;

    bool operator!=(const Vector3& v) const;

    Vector3 operator*(const Vector3& v) const;
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);

using Point3 = Vector3;


#endif
