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

    Vector3 GetPosition() { return Vector3(vX.data[3], vY.data[3], vZ.data[3]); }
    Vector3 GetScale() { return Vector3(vX.data[0], vY.data[1], vZ.data[2]); }
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
    vX = { temp.vX.data[0], temp.vY.data[0], temp.vZ.data[0], temp.vW.data[0] };
    vY = { temp.vX.data[1], temp.vY.data[1], temp.vZ.data[1], temp.vW.data[1] };
    vZ = { temp.vX.data[2], temp.vY.data[2], temp.vZ.data[2], temp.vW.data[2] };
    vW = { temp.vX.data[3], temp.vY.data[3], temp.vZ.data[3], temp.vW.data[3] };
}

inline void Matrix4x4::SetOrtho(float width, float height, float near, float far)
{
    //The following docs are in row major, this matrix uses column major
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh Left handed, USE_LEFT_HANDED_MATRICES = true
    //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh Right handed, USE_LEFT_HANDED_MATRICES = false
    Identity();
    if constexpr (USE_LEFT_HANDED_MATRICES)
    {
        vX.data[0] = 2 / width;
        vY.data[1] = 2 / height;
        vZ.data[2] = 1 / (far - near);
        vZ.data[3] = -near / (far - near);
    }
    else
    {
        vX.data[0] = 2 / width;
        vY.data[1] = 2 / height;
        vZ.data[2] = 1 / (near - far);
        vZ.data[3] = near / (near - far);
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
        vX.data[0] = xScale;
        vY.data[1] = yScale;
        vZ.data[2] = far / (far - near);
        vZ.data[3] = (-near * far) / (far - near);
        vW.data[2] = 1;
        vW.data[3] = 0;
    }
    else
    {
        vX.data[0] = xScale;
        vY.data[1] = yScale;
        vZ.data[2] = far / (near - far);
        vZ.data[3] = (near * far) / (near - far);
        vW.data[2] = -1;
        vW.data[3] = 0;
    }
}

inline void Matrix4x4::SetPosition(Vector3 position)
{
    vX.data[3] = position.x;
    vY.data[3] = position.y;
    vZ.data[3] = position.z;
}

inline void Matrix4x4::Translate(Vector3 position)
{
    vX.data[3] += position.x;
    vY.data[3] += position.y;
    vZ.data[3] += position.z;
}

inline void Matrix4x4::RotateX(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vY.data[1] = cosAngle;
    rotMatrix.vY.data[2] = sinAngle;
    rotMatrix.vZ.data[1] = -sinAngle;
    rotMatrix.vZ.data[2] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::RotateY(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vX.data[0] = cosAngle;
    rotMatrix.vX.data[2] = sinAngle;
    rotMatrix.vZ.data[0] = -sinAngle;
    rotMatrix.vZ.data[2] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::RotateZ(float degrees)
{
    Matrix4x4 rotMatrix;
    float radians = degrees / 180.f * static_cast<float>(PI);

    float sinAngle = sinf(radians);
    float cosAngle = cosf(radians);

    rotMatrix.vX.data[0] = cosAngle;
    rotMatrix.vX.data[1] = -sinAngle;
    rotMatrix.vY.data[0] = sinAngle;
    rotMatrix.vY.data[1] = cosAngle;

    *this *= rotMatrix;
}

inline void Matrix4x4::SetScale(Vector3 scale)
{
    vX.data[0] = scale.x;
    vY.data[1] = scale.y;
    vZ.data[2] = scale.z;
}

inline void Matrix4x4::Scale(Vector3 scale)
{
    vX.data[0] += scale.x;
    vY.data[1] += scale.y;
    vZ.data[2] += scale.z;
}

inline Vector3 Matrix4x4::GetEulerAngles()
{
    float radToDeg = 180 / static_cast<float>(PI);
    if (vY.data[2] > 1.0f)
    {
        float x = static_cast<float>(PI) / 2;
        float y = atan2f(vX.data[1], vX.data[0]);
        float z = 0.0f;
        return Vector3(x, y, z) * radToDeg;
    }
    else if (vY.data[2] < -1.0f)
    {
        float x = static_cast<float>(PI) / 2;
        float y = -atan2f(vX.data[1], vX.data[0]);
        float z = 0.0f;
        return Vector3(x, y, z) * radToDeg;
    }
    else
    {
        float x = asinf(vY.data[2]);
        float y = atan2f(-vX.data[2], vZ.data[2]);
        float z = atan2f(-vY.data[0], vY.data[1]);
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

    output.vX.data[0] = vX.Dot(mat.vX);
    output.vX.data[1] = vX.Dot(mat.vY);
    output.vX.data[2] = vX.Dot(mat.vZ);
    output.vX.data[3] = vX.Dot(mat.vW);

    output.vY.data[0] = vY.Dot(mat.vX);
    output.vY.data[1] = vY.Dot(mat.vY);
    output.vY.data[2] = vY.Dot(mat.vZ);
    output.vY.data[3] = vY.Dot(mat.vW);

    output.vZ.data[0] = vZ.Dot(mat.vX);
    output.vZ.data[1] = vZ.Dot(mat.vY);
    output.vZ.data[2] = vZ.Dot(mat.vZ);
    output.vZ.data[3] = vZ.Dot(mat.vW);

    output.vW.data[0] = vW.Dot(mat.vX);
    output.vW.data[1] = vW.Dot(mat.vY);
    output.vW.data[2] = vW.Dot(mat.vZ);
    output.vW.data[3] = vW.Dot(mat.vW);

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

    vX.data[0] = copy.vX.Dot(mat.vX);
    vX.data[1] = copy.vX.Dot(mat.vY);
    vX.data[2] = copy.vX.Dot(mat.vZ);
    vX.data[3] = copy.vX.Dot(mat.vW);

    vY.data[0] = copy.vY.Dot(mat.vX);
    vY.data[1] = copy.vY.Dot(mat.vY);
    vY.data[2] = copy.vY.Dot(mat.vZ);
    vY.data[3] = copy.vY.Dot(mat.vW);

    vZ.data[0] = copy.vZ.Dot(mat.vX);
    vZ.data[1] = copy.vZ.Dot(mat.vY);
    vZ.data[2] = copy.vZ.Dot(mat.vZ);
    vZ.data[3] = copy.vZ.Dot(mat.vW);

    vW.data[0] = copy.vW.Dot(mat.vX);
    vW.data[1] = copy.vW.Dot(mat.vY);
    vW.data[2] = copy.vW.Dot(mat.vZ);
    vW.data[3] = copy.vW.Dot(mat.vW);
}


#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far
