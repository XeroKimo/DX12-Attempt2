#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"

struct Quaternion
{
    float r;
    float i;
    float j;
    float k;

public:
    Quaternion();
    Quaternion(float x, float y, float z, float w);
    void Identity();

    void Rotate(Vector3 axis, float angle);

    Quaternion operator+ (const Quaternion& other);
    Quaternion operator- (const Quaternion& other);
    Quaternion operator* (const Quaternion& other);
    Quaternion operator/ (const Quaternion& other);

    void operator+= (const Quaternion& other);
    void operator-= (const Quaternion& other);
    void operator*= (const Quaternion& other);
    void operator/= (const Quaternion& other);

    void Normalize();

    Matrix4x4 GetRotation();
};

inline Quaternion::Quaternion()
{
    Identity();
}

inline Quaternion::Quaternion(float x, float y, float z, float w)
{
    r = x;
    i = y;
    j = z;
    k = w;
}

inline void Quaternion::Identity()
{
    r = 1;
    i = 0;
    j = 0;
    k = 0;
}

inline void Quaternion::Rotate(Vector3 axis, float angle)
{
    angle = angle / 180 * static_cast<float>(PI) / 2;
    Quaternion temp;
    float sinAngle = sin(angle);
    temp.r = cos(angle);
    temp.i = axis.x * sinAngle;
    temp.j = axis.y * sinAngle;
    temp.k = axis.z * sinAngle;

    *this = *this * temp;
}

inline Quaternion Quaternion::operator+(const Quaternion& other)
{
    Quaternion result;
    result.r = r + other.r;
    result.i = i + other.i;
    result.j = j + other.j;
    result.k = k + other.k;

    return result;
}

inline Quaternion Quaternion::operator-(const Quaternion& other)
{
    Quaternion result;
    result.r = r - other.r;
    result.i = i - other.i;
    result.j = j - other.j;
    result.k = k - other.k;

    return result;
}

inline Quaternion Quaternion::operator*(const Quaternion& other)
{
    Quaternion result;
    Matrix4x4 mat
    (
        Vector4(r, -i, -j, -k),
        Vector4(i, r, -k, j),
        Vector4(j, k, r, -i),
        Vector4(k, -j, i, r)
    );
    Vector4 vec(other.r, other.i, other.j, other.k);
    Vector4 res = mat * vec;

    result = Quaternion(res.x, res.y, res.z, res.w);

    return result;
}

inline Quaternion Quaternion::operator/(const Quaternion& other)
{
    Quaternion result;
    float dividor = other.r * other.r + other.i * other.i + other.j * other.j + other.k * other.k;

    result.r = (other.r * r + other.i * i + other.j * j + other.k * k) / dividor;
    result.i = (other.r * i - other.i * r - other.j * k + other.k * j) / dividor;
    result.j = (other.r * j + other.i * k - other.j * r - other.k * i) / dividor;
    result.k = (other.r * k - other.i * j + other.j * i - other.k * r) / dividor;


    return result;
}

inline void Quaternion::operator+=(const Quaternion& other)
{
    r += other.r;
    i += other.i;
    j += other.j;
    k += other.k;
}

inline void Quaternion::operator-=(const Quaternion& other)
{
    r -= other.r;
    i -= other.i;
    j -= other.j;
    k -= other.k;
}

inline void Quaternion::operator*=(const Quaternion& other)
{
    Matrix4x4 mat
    (
        Vector4(r, -i, -j, -k),
        Vector4(i, r, -k, j),
        Vector4(j, k, r, -i),
        Vector4(k, -j, i, r)
    );
    Vector4 vec(other.r, other.i, other.j, other.k);
    Vector4 res = mat * vec;

    *this = Quaternion(res.x, res.y, res.z, res.w);
}

inline void Quaternion::operator/=(const Quaternion& other)
{
    Quaternion result;
    float dividor = other.r * other.r + other.i * other.i + other.j * other.j + other.k * other.k;

    result.r = (other.r * r + other.i * i + other.j * j + other.k * k) / dividor;
    result.i = (other.r * i - other.i * r - other.j * k + other.k * j) / dividor;
    result.j = (other.r * j + other.i * k - other.j * r - other.k * i) / dividor;
    result.k = (other.r * k - other.i * j + other.j * i - other.k * r) / dividor;

    *this = result;
}

inline void Quaternion::Normalize()
{
    Vector4 normalized(r, i, j, k);
    normalized.Normalize();
    r = normalized.x;
    i = normalized.y;
    j = normalized.z;
    k = normalized.w;
}

//https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Quaternion-derived_rotation_matrix
inline Matrix4x4 Quaternion::GetRotation()
{
    Matrix4x4 matrix;
    matrix.vX.x = 1 - 2 * (j * j + k * k);
    matrix.vX.y = 2 * (i * j -  r * k);
    matrix.vX.z = 2 * (i * k + r * j);

    matrix.vY.x = 2 * (i * j +  r * k);
    matrix.vY.y = 1 - 2 * (i * i + k * k);
    matrix.vY.z = 2 * (k * j - r * i);

    matrix.vZ.x = 2 * (i * k - r * j);
    matrix.vZ.y = 2 * (j * k + r * i);
    matrix.vZ.z = 1 - 2 * (i * i + j * j);

    return matrix;
}

//struct Quaternion
//{
//    Vector4 va;
//    Vector4 vY;
//    Vector4 vZ;
//    Vector4 vW;
//
//public:
//    Quaternion();
//
//    void Identity();
//};
//
//inline Quaternion::Quaternion()
//{
//    Identity();
//}
//
//inline void Quaternion::Identity()
//{
//
//}
