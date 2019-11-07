#pragma once
#include "MathConstants.h"
#include "Vector4.h"

#ifdef near
#undef near
#endif // near

#ifdef far
#undef far
#endif // far

struct Matrix4x4
{
public:

    Matrix4x4();
    Matrix4x4(Vector4 x, Vector4 y, Vector4 z, Vector4 w);

    void Identity();
    void Transpose();
    void SetOrtho(float width, float height, float near, float far);
    void SetPerspective(float fovAngleY, float aspectRatio, float near, float far);

    void SetPosition(Vector3 position);
    void Translate(Vector3 position);
    void RotateX(float degrees);
    void RotateY(float degrees);
    void RotateZ(float degrees);
    void SetScale(Vector3 scale);
    void Scale(Vector3 scale);

    Vector3 GetPosition() { return Vector3(vX.value[3], vY.value[3], vZ.value[3]); }
    Vector3 GetScale() { return Vector3(vX.value[0], vY.value[1], vZ.value[2]); }
    Vector3 GetEulerAngles();

    Matrix4x4 GetTransposed();

    Matrix4x4 operator+(const Matrix4x4& other);
    Matrix4x4 operator-(const Matrix4x4& other);
    Matrix4x4 operator*(const Matrix4x4& other);

    Vector4 operator*(const Vector4& other);

    void operator+=(const Matrix4x4& other);
    void operator-=(const Matrix4x4& other);
    void operator*=(const Matrix4x4& other);
public:
    Vector4 vX;
    Vector4 vY;
    Vector4 vZ;
    Vector4 vW;
};

inline Matrix4x4::Matrix4x4()
{
    Identity();
}

inline Matrix4x4::Matrix4x4(Vector4 x, Vector4 y, Vector4 z, Vector4 w)
{
    vX = x;
    vY = y;
    vZ = z;
    vW = w;
}

void inline Matrix4x4::Identity()
{
    vX = { 1.0f, 0.0f, 0.0f, 0.0f };
    vY = { 0.0f, 1.0f, 0.0f, 0.0f };
    vZ = { 0.0f, 0.0f, 1.0f, 0.0f };
    vW = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void inline Matrix4x4::Transpose()
{
    Matrix4x4 temp = *this;
    vX = { temp.vX.value[0], temp.vY.value[0], temp.vZ.value[0], temp.vW.value[0] };
    vY = { temp.vX.value[1], temp.vY.value[1], temp.vZ.value[1], temp.vW.value[1] };
    vZ = { temp.vX.value[2], temp.vY.value[2], temp.vZ.value[2], temp.vW.value[2] };
    vW = { temp.vX.value[3], temp.vY.value[3], temp.vZ.value[3], temp.vW.value[3] };
}

inline void Matrix4x4::SetOrtho(float width, float height, float near, float far)
{
    //The following docs are in row major, this matrix uses column major
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh Left handed, USE_LEFT_HANDED_MATRICES = true
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh Right handed, USE_LEFT_HANDED_MATRICES = false
    Identity();
    if constexpr (USE_LEFT_HANDED_MATRICES)
    {
        vX.value[0] = 2 / width;
        vY.value[1] = 2 / height;
        vZ.value[2] = 1 / (far - near);
        vZ.value[3] = -near / (far - near);
    }
    else
    {
        vX.value[0] = 2 / width;
        vY.value[1] = 2 / height;
        vZ.value[2] = 1 / (near - far);
        vZ.value[3] = near / (near - far);
    }
}

inline void Matrix4x4::SetPerspective(float fovAngleY, float aspectRatio, float near, float far)
{
    //The following docs are in row major, this matrix uses column major
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh Left Handed, posZIn = true
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh Right handed, posZIn = false
    float yScale = 1 / tanf(fovAngleY * 3.14f / 180.0f * 0.5f);
    float xScale = yScale / aspectRatio;

    Identity();
    if constexpr (USE_LEFT_HANDED_MATRICES)
    {
        vX.value[0] = xScale;
        vY.value[1] = yScale;
        vZ.value[2] = far / (far - near);
        vZ.value[3] = (-near * far) / (far - near);
        vW.value[2] = 1;
        vW.value[3] = 0;
    }
    else
    {
        vX.value[0] = xScale;
        vY.value[1] = yScale;
        vZ.value[2] = far / (near - far);
        vZ.value[3] = (near * far) / (near - far);
        vW.value[2] = -1;
        vW.value[3] = 0;
    }
}

inline void Matrix4x4::SetPosition(Vector3 position)
{
    vX.value[3] = position.x;
    vY.value[3] = position.y;
    vZ.value[3] = position.z;
}

inline void Matrix4x4::Translate(Vector3 position)
{
    vX.value[3] += position.x;
    vY.value[3] += position.y;
    vZ.value[3] += position.z;
}

inline void Matrix4x4::RotateX(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vY.value[1] = cosAngle;
    rotMatrix.vY.value[2] = sinAngle;
    rotMatrix.vZ.value[1] = -sinAngle;
    rotMatrix.vZ.value[2] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::RotateY(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vX.value[0] = cosAngle;
    rotMatrix.vX.value[2] = sinAngle;
    rotMatrix.vZ.value[0] = -sinAngle;
    rotMatrix.vZ.value[2] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::RotateZ(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vX.value[0] = cosAngle;
    rotMatrix.vX.value[1] = -sinAngle;
    rotMatrix.vY.value[0] = sinAngle;
    rotMatrix.vY.value[1] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::SetScale(Vector3 scale)
{
    vX.value[0] = scale.x;
    vY.value[1] = scale.y;
    vZ.value[2] = scale.z;
}

inline void Matrix4x4::Scale(Vector3 scale)
{
    vX.value[0] += scale.x;
    vY.value[1] += scale.y;
    vZ.value[2] += scale.z;
}

inline Vector3 Matrix4x4::GetEulerAngles()
{
    float radToDeg = 180 / static_cast<float>(PI);
    if (vY.value[2] > 1.0f)
    {
        float x = static_cast<float>(PI) / 2;
        float y = atan2f(vX.value[1], vX.value[0]);
        float z = 0.0f;
        return Vector3(x, y, z) * radToDeg;
    }
    else if (vY.value[2] < -1.0f)
    {
        float x = static_cast<float>(PI) / 2;
        float y = -atan2f(vX.value[1], vX.value[0]);
        float z = 0.0f;
        return Vector3(x, y, z) * radToDeg;
    }
    else
    {
        float x = asinf(vY.value[2]);
        float y = atan2f(-vX.value[2], vZ.value[2]);
        float z = atan2f(-vY.value[0], vY.value[1]);
        return Vector3(x, y, z) * radToDeg;
    }
}

inline Matrix4x4 Matrix4x4::GetTransposed()
{
    Matrix4x4 temp = *this;
    temp.Transpose();
    return temp;
}

Matrix4x4 inline Matrix4x4::operator+(const Matrix4x4& other)
{
    Matrix4x4 mat;
    mat.vX = vX + other.vX;
    mat.vY = vY + other.vY;
    mat.vZ = vZ + other.vZ;
    mat.vW = vW + other.vW;

    return mat;
}

Matrix4x4 inline Matrix4x4::operator-(const Matrix4x4& other)
{
    Matrix4x4 mat;
    mat.vX = vX - other.vX;
    mat.vY = vY - other.vY;
    mat.vZ = vZ - other.vZ;
    mat.vW = vW - other.vW;

    return mat;
}

Matrix4x4 inline Matrix4x4::operator*(const Matrix4x4& other)
{
    Matrix4x4 mat = other;
    mat.Transpose();
    Matrix4x4 output;

    output.vX.value[0] = vX.Dot(mat.vX);
    output.vX.value[1] = vX.Dot(mat.vY);
    output.vX.value[2] = vX.Dot(mat.vZ);
    output.vX.value[3] = vX.Dot(mat.vW);

    output.vY.value[0] = vY.Dot(mat.vX);
    output.vY.value[1] = vY.Dot(mat.vY);
    output.vY.value[2] = vY.Dot(mat.vZ);
    output.vY.value[3] = vY.Dot(mat.vW);

    output.vZ.value[0] = vZ.Dot(mat.vX);
    output.vZ.value[1] = vZ.Dot(mat.vY);
    output.vZ.value[2] = vZ.Dot(mat.vZ);
    output.vZ.value[3] = vZ.Dot(mat.vW);

    output.vW.value[0] = vW.Dot(mat.vX);
    output.vW.value[1] = vW.Dot(mat.vY);
    output.vW.value[2] = vW.Dot(mat.vZ);
    output.vW.value[3] = vW.Dot(mat.vW);

    return output;
}

Vector4 inline Matrix4x4::operator*(const Vector4& other)
{
    float x = vX.Dot(other);
    float y = vY.Dot(other);
    float z = vZ.Dot(other);
    float w = vW.Dot(other);

    return Vector4(x, y, z, w);
}

void inline Matrix4x4::operator+=(const Matrix4x4& other)
{
    vX += other.vX;
    vY += other.vY;
    vZ += other.vZ;
    vW += other.vW;
}

void inline Matrix4x4::operator-=(const Matrix4x4& other)
{
    vX -= other.vX;
    vY -= other.vY;
    vZ -= other.vZ;
    vW -= other.vW;
}

void inline Matrix4x4::operator*=(const Matrix4x4& other)
{
    Matrix4x4 mat = other;
    mat.Transpose();
    Matrix4x4 copy = *this;

    vX.value[0] = copy.vX.Dot(mat.vX);
    vX.value[1] = copy.vX.Dot(mat.vY);
    vX.value[2] = copy.vX.Dot(mat.vZ);
    vX.value[3] = copy.vX.Dot(mat.vW);

    vY.value[0] = copy.vY.Dot(mat.vX);
    vY.value[1] = copy.vY.Dot(mat.vY);
    vY.value[2] = copy.vY.Dot(mat.vZ);
    vY.value[3] = copy.vY.Dot(mat.vW);

    vZ.value[0] = copy.vZ.Dot(mat.vX);
    vZ.value[1] = copy.vZ.Dot(mat.vY);
    vZ.value[2] = copy.vZ.Dot(mat.vZ);
    vZ.value[3] = copy.vZ.Dot(mat.vW);

    vW.value[0] = copy.vW.Dot(mat.vX);
    vW.value[1] = copy.vW.Dot(mat.vY);
    vW.value[2] = copy.vW.Dot(mat.vZ);
    vW.value[3] = copy.vW.Dot(mat.vW);
}


#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far
