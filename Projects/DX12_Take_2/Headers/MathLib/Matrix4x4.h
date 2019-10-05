#pragma once
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
	Matrix4x4(Vector4 one, Vector4 two, Vector4 three, Vector4 four);

	void Identity();
	void Transpose();
	void SetOrtho(float width, float height, float near, float far, bool posZIn);
	void SetPerspective(float fovAngleY, float aspectRatio, float near, float far, bool posZIn);

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
	vX = { temp.vX.x, temp.vY.x, temp.vZ.x, temp.vW.x };
	vY = { temp.vX.y, temp.vY.y, temp.vZ.y, temp.vW.y };
	vZ = { temp.vX.z, temp.vY.z, temp.vZ.z, temp.vW.z };
	vW = { temp.vX.w, temp.vY.w, temp.vZ.w, temp.vW.w };
}

inline void Matrix4x4::SetOrtho(float width, float height, float near, float far, bool posZIn)
{
	//The following docs are in row major, this matrix uses column major
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh Left handed, posZIn = true
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh Right handed, posZIn = false
	Identity();
	if (posZIn)
	{
		vX.x = 2 / width;
		vY.y = 2 / height;
		vZ.z = 1 / (far - near);
		vZ.w = -near / (far - near);
	}
	else
	{
		vX.x = 2 / width;
		vY.y = 2 / height;
		vZ.z = 1 / (near - far);
		vZ.w = near / (near - far);
	}
}

inline void Matrix4x4::SetPerspective(float fovAngleY, float aspectRatio, float near, float far, bool posZIn)
{
	//The following docs are in row major, this matrix uses column major
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh Left Handed, posZIn = true
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh Right handed, posZIn = false
	float yScale = 1 / tanf(fovAngleY * 3.14f / 180.0f * 0.5f);
	float xScale = yScale / aspectRatio;

	Identity();
	if (posZIn)
	{
		vX.x = xScale;
		vY.y = yScale;
		vZ.z = far / (far - near);
		vZ.w = (-near * far) / (far - near);
		vW.z = 1;
		vW.w = 0;
	}
	else
	{
		vX.x = xScale;
		vY.y = yScale;
		vZ.z = far / (near - far);
		vZ.w = (near * far) / (near - far);
		vW.z = -1;
		vW.w = 0;
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

	mat.vX.x = vX.Dot(mat.vX);
	mat.vX.y = vX.Dot(mat.vY);
	mat.vX.z = vX.Dot(mat.vZ);
	mat.vX.w = vX.Dot(mat.vW);

	mat.vY.x = vY.Dot(mat.vX);
	mat.vY.y = vY.Dot(mat.vY);
	mat.vY.z = vY.Dot(mat.vZ);
	mat.vY.w = vY.Dot(mat.vW);

	mat.vZ.x = vZ.Dot(mat.vX);
	mat.vZ.y = vZ.Dot(mat.vY);
	mat.vZ.z = vZ.Dot(mat.vZ);
	mat.vZ.w = vZ.Dot(mat.vW);

	mat.vW.x = vW.Dot(mat.vX);
	mat.vW.y = vW.Dot(mat.vY);
	mat.vW.z = vW.Dot(mat.vZ);
	mat.vW.w = vW.Dot(mat.vW);

	return mat;
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

    vX.x = vX.Dot(mat.vX);
    vX.y = vX.Dot(mat.vY);
    vX.z = vX.Dot(mat.vZ);
    vX.w = vX.Dot(mat.vW);
    
    vY.x = vY.Dot(mat.vX);
    vY.y = vY.Dot(mat.vY);
    vY.z = vY.Dot(mat.vZ);
    vY.w = vY.Dot(mat.vW);
    
    vZ.x = vZ.Dot(mat.vX);
    vZ.y = vZ.Dot(mat.vY);
    vZ.z = vZ.Dot(mat.vZ);
    vZ.w = vZ.Dot(mat.vW);
    
    vW.x = vW.Dot(mat.vX);
    vW.y = vW.Dot(mat.vY);
    vW.z = vW.Dot(mat.vZ);
    vW.w = vW.Dot(mat.vW);
}


#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far
