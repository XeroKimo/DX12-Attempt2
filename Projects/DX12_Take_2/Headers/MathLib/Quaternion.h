#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"

struct Quaternion
{
    float a;
    float b;
    float c;
    float d;

public:
    Quaternion();
    Quaternion(float x, float y, float z, float w);
    void Identity();

    void Rotate(Vector3 axis, float angle);

    Quaternion operator+ (const Quaternion& other);
    Quaternion operator- (const Quaternion& other);
    Quaternion operator* (const Quaternion& other);
    Quaternion operator/ (const Quaternion& other);

    void Normalize();

    Matrix4x4 GetRotation();
};

inline Quaternion::Quaternion()
{
    Identity();
}

inline Quaternion::Quaternion(float x, float y, float z, float w)
{
    a = x;
    b = y;
    c = z;
    d = w;
}

inline void Quaternion::Identity()
{
    a = 1;
    b = 0;
    c = 0;
    d = 0;
}

inline void Quaternion::Rotate(Vector3 axis, float angle)
{
    angle = angle / 180 * PI / 2;
    Quaternion temp;
    float sinAngle = sin(angle);
    temp.a = cos(angle);
    temp.b = axis.x * sinAngle;
    temp.c = axis.y * sinAngle;
    temp.d = axis.z * sinAngle;

    *this = *this * temp;
}

inline Quaternion Quaternion::operator+(const Quaternion& other)
{
    Quaternion result;
    result.a = a + other.a;
    result.b = b + other.b;
    result.c = c + other.c;
    result.d = d + other.d;

    return result;
}

inline Quaternion Quaternion::operator-(const Quaternion& other)
{
    Quaternion result;
    result.a = a - other.a;
    result.b = b - other.b;
    result.c = c - other.c;
    result.d = d - other.d;

    return result;
}

inline Quaternion Quaternion::operator*(const Quaternion& other)
{
    Quaternion result;
    result.a = (a * other.a) - (b * other.b) - (c * other.c) - (d * other.d);
    result.b = (b * other.a) + (a * other.b) - (d * other.c) + (c * other.d);
    result.c = (c * other.a) + (d * other.b) + (a * other.c) - (b * other.d);
    result.d = (d * other.a) - (c * other.b) + (b * other.c) + (a * other.d);

    return result;
}

inline Quaternion Quaternion::operator/(const Quaternion& other)
{
    Quaternion result;
    float dividor = other.a * other.a + other.b * other.b + other.c * other.c + other.d * other.d;

    result.a = (other.a * a + other.b * b + other.c * c + other.d * d) / dividor;
    result.b = (other.a * b - other.b * a - other.c * d + other.d * c) / dividor;
    result.c = (other.a * c + other.b * d - other.c * a - other.d * b) / dividor;
    result.d = (other.a * d - other.b * c + other.c * b - other.d * a) / dividor;


    return result;
}

inline void Quaternion::Normalize()
{
    Vector4 normalized(a, b, c, d);
    normalized.Normalize();
    a = normalized.x;
    b = normalized.y;
    c = normalized.z;
    d = normalized.w;
}

inline Matrix4x4 Quaternion::GetRotation()
{
    Matrix4x4 matrix;
    matrix.vX.x = 1 - 2 * (c * c) - 2 * (d * d);
    matrix.vX.y = (2 * b * c) - (2 * a * d);
    matrix.vX.z = (2 * b * d) + (2 * a * c);

    matrix.vY.x = (2 * b * c) + (2 * a * d);
    matrix.vY.y = 1 - 2 * (b * b) - 2 * (d * d);
    matrix.vY.z = (2 * d * c) + (2 * a * b);

    matrix.vZ.x = (2 * b * d) - (2 * a * c);
    matrix.vZ.y = (2 * c * d) - (2 * a * b);
    matrix.vZ.z = 1 - 2 * (b * b) - 2 * (c * c);

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
