#include "vector.hh"
#include <cmath>
#include <ostream>

Vector3 Vector3::operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

Vector3 Vector3::operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

Vector3 Vector3::operator*(float t) const {
        return Vector3(x * t, y * t, z * t);
    }

Vector3 Vector3::operator/(float t) const {
        return Vector3(x / t, y / t, z / t);
    }

// produit scalaire
float Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// length
float Vector3::norm() const {
    return std::sqrt(dot(*this));
}

// produit vectoriel
Vector3 Vector3::p_v(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

bool Vector3::operator!=(const Vector3& v) const {
    return x != v.x || y != v.y || z != v.z;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << v.x << " " << v.y << " " << v.z;
    return os;
}

Vector3 Vector3::operator*(const Vector3& v) const {
    return Vector3(x * v.x, y * v.y, z * v.z);
}
