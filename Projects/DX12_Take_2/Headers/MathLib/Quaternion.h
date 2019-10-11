#pragma once
#include "Vector4.h"

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;

public:
    Quaternion();
    void Identity();

    Quaternion operator+ (const Quaternion& other);
    Quaternion operator* (const Quaternion& other);

};

inline Quaternion::Quaternion()
{
    Identity();
}

inline void Quaternion::Identity()
{
    x = 1;
    y = 0;
    z = 0;
    w = 0;
}

inline Quaternion Quaternion::operator+(const Quaternion& other)
{
    Quaternion result;
    result.x = x + other.x;
    result.y = y + other.y;
    result.z = z + other.z;
    result.w = w + other.w;

    return result;
}

inline Quaternion Quaternion::operator*(const Quaternion& other)
{
    Quaternion result;
    result.x = (x * other.x) - (y * other.y) - (z * other.z) - (w * other.w);
    result.y = (y * other.x) + (x * other.y) - (w * other.z) + (z * other.w);
    result.z = (z * other.x) + (w * other.y) + (x * other.z) - (y * other.w);
    result.w = (w * other.x) - (z * other.y) + (y * other.z) + (x * other.w);

    return result;
}

//struct Quaternion
//{
//    Vector4 vX;
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
