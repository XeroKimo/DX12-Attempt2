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

	Matrix4x4 operator+(const Matrix4x4& other);
	Matrix4x4 operator-(const Matrix4x4& other);
	Matrix4x4 operator*(const Matrix4x4& other);

	Vector4 operator*(const Vector4& other);

	void operator+=(const Matrix4x4& other);
	void operator-=(const Matrix4x4& other);
	void operator*=(const Matrix4x4& other);
public:
	float v1x, v2x, v3x, v4x;
	float v1y, v2y, v3y, v4y;
	float v1z, v2z, v3z, v4z;
	float v1w, v2w, v3w, v4w;
};

inline Matrix4x4::Matrix4x4()
{
	Identity();
}

inline Matrix4x4::Matrix4x4(Vector4 one, Vector4 two, Vector4 three, Vector4 four)
{
	v1x = one.x;
	v1y = one.y;
	v1z = one.z;
	v1w = one.w;

	v2x = two.x;
	v2y = two.y;
	v2z = two.z;
	v2w = two.w;

	v3x = three.x;
	v3y = three.y;
	v3z = three.z;
	v3w = three.w;

	v4x = four.x;
	v4y = four.y;
	v4z = four.z;
	v4w = four.w;
}

void inline Matrix4x4::Identity()
{
	v1x = v2y = v3z = v4w = 1.0f;
	v1y = v1z = v1w = 0.0f;
	v2x = v2z = v2w = 0.0f;
	v3x = v3y = v3w = 0.0f;
	v4x = v4y = v4z = 0.0f;
}

void inline Matrix4x4::Transpose()
{
	Matrix4x4 temp = *this;
	v1x = temp.v1x;
	v1y = temp.v2x;
	v1z = temp.v3x;
	v1w = temp.v4x;

	v2x = temp.v1y;
	v2y = temp.v2y;
	v2z = temp.v3y;
	v2w = temp.v4y;

	v3x = temp.v1z;
	v3y = temp.v2z;
	v3z = temp.v3z;
	v3w = temp.v4z;

	v4x = temp.v1w;
	v4y = temp.v2w;
	v4z = temp.v3w;
	v4w = temp.v4w;
}

inline void Matrix4x4::SetOrtho(float width, float height, float near, float far, bool posZIn)
{
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh
	Identity();
	if (posZIn)
	{
		v1x = 2 / width;
		v2y = 2 / height;
		v3z = 1 / (far - near);
		v4z = -near / (far - near);
	}
	else
	{
		v1x = 2 / width;
		v2y = 2 / height;
		v3z = 1 / (near - far);
		v4z = near / (near - far);
	}
}

inline void Matrix4x4::SetPerspective(float fovAngleY, float aspectRatio, float near, float far, bool posZIn)
{
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh LeftHanded, posZIn = true
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh Right handed, posZIn = false
	float yScale = 1 / tanf(fovAngleY * 3.14f / 180.0f * 0.5f);
	float xScale = yScale / aspectRatio;

	Identity();
	if (posZIn)
	{
		v1x = xScale;
		v2y = yScale;
		v3z = far / (far - near);
		v3w = 1;
		v3z = (-near * far) / (far - near);
		v4w = 0;
	}
	else
	{
		v1x = xScale;
		v2y = yScale;
		v3z = far / (near - far);
		v3w = -1;
		v3z = (near * far) / (near - far);
		v4w = 0;
	}
}

Matrix4x4 inline Matrix4x4::operator+(const Matrix4x4& other)
{
	Matrix4x4 mat;
	mat.v1x = v1x + other.v1x;
	mat.v2x = v2x + other.v2x;
	mat.v3x = v3x + other.v3x;
	mat.v4x = v4x + other.v4x;

	mat.v1y = v1y + other.v1y;
	mat.v2y = v2y + other.v2y;
	mat.v3y = v3y + other.v3y;
	mat.v4y = v4y + other.v4y;

	mat.v1z = v1z + other.v1z;
	mat.v2z = v2z + other.v2z;
	mat.v3z = v3z + other.v3z;
	mat.v4z = v4z + other.v4z;

	mat.v1w = v1w + other.v1w;
	mat.v2w = v2w + other.v2w;
	mat.v3w = v3w + other.v3w;
	mat.v4w = v4w + other.v4w;

	return mat;
}

Matrix4x4 inline Matrix4x4::operator-(const Matrix4x4& other)
{
	Matrix4x4 mat;
	mat.v1x = v1x - other.v1x;
	mat.v2x = v2x - other.v2x;
	mat.v3x = v3x - other.v3x;
	mat.v4x = v4x - other.v4x;
				  
	mat.v1y = v1y - other.v1y;
	mat.v2y = v2y - other.v2y;
	mat.v3y = v3y - other.v3y;
	mat.v4y = v4y - other.v4y;
				  
	mat.v1z = v1z - other.v1z;
	mat.v2z = v2z - other.v2z;
	mat.v3z = v3z - other.v3z;
	mat.v4z = v4z - other.v4z;
				  
	mat.v1w = v1w - other.v1w;
	mat.v2w = v2w - other.v2w;
	mat.v3w = v3w - other.v3w;
	mat.v4w = v4w - other.v4w;

	return mat;
}

Matrix4x4 inline Matrix4x4::operator*(const Matrix4x4& other)
{
	Matrix4x4 mat;
	Vector4 x = Vector4(v1x, v2x, v3x, v4x);
	Vector4 y = Vector4(v1y, v2y, v3y, v4y);
	Vector4 z = Vector4(v1z, v2z, v3z, v4z);
	Vector4 w = Vector4(v1w, v2w, v3w, v4w);

	Vector4 one = Vector4(other.v1x, other.v1y, other.v1z, other.v1w);
	Vector4 two = Vector4(other.v2x, other.v2y, other.v2z, other.v2w);
	Vector4 three = Vector4(other.v3x, other.v3y, other.v3z, other.v3w);
	Vector4 four = Vector4(other.v4x, other.v4y, other.v4z, other.v4w);

	mat.v1x = x.Dot(one);
	mat.v2x = x.Dot(two);
	mat.v3x = x.Dot(three);
	mat.v4x = x.Dot(four);

	mat.v1y = y.Dot(one);
	mat.v2y = y.Dot(two);
	mat.v3y = y.Dot(three);
	mat.v4y = y.Dot(four);

	mat.v1z = z.Dot(one);
	mat.v2z = z.Dot(two);
	mat.v3z = z.Dot(three);
	mat.v4z = z.Dot(four);

	mat.v1w = w.Dot(one);
	mat.v2w = w.Dot(two);
	mat.v3w = w.Dot(three);
	mat.v4w = w.Dot(four);

	return mat;
}

Vector4 inline Matrix4x4::operator*(const Vector4& other)
{
	float x = Vector4(v1x, v2x, v3x, v4x).Dot(other);
	float y = Vector4(v1y, v2y, v3y, v4y).Dot(other);
	float z = Vector4(v1z, v2z, v3z, v4z).Dot(other);
	float w = Vector4(v1w, v2w, v3w, v4w).Dot(other);

	return Vector4(x, y, z, w);
}

void inline Matrix4x4::operator+=(const Matrix4x4& other)
{
	v1x += other.v1x;
	v2x += other.v2x;
	v3x += other.v3x;
	v4x += other.v4x;

	v1y += other.v1y;
	v2y += other.v2y;
	v3y += other.v3y;
	v4y += other.v4y;

	v1z += other.v1z;
	v2z += other.v2z;
	v3z += other.v3z;
	v4z += other.v4z;

	v1w += other.v1w;
	v2w += other.v2w;
	v3w += other.v3w;
	v4w += other.v4w;
}

void inline Matrix4x4::operator-=(const Matrix4x4& other)
{
	v1x -= other.v1x;
	v2x -= other.v2x;
	v3x -= other.v3x;
	v4x -= other.v4x;
		
	v1y -= other.v1y;
	v2y -= other.v2y;
	v3y -= other.v3y;
	v4y -= other.v4y;
		
	v1z -= other.v1z;
	v2z -= other.v2z;
	v3z -= other.v3z;
	v4z -= other.v4z;
		
	v1w -= other.v1w;
	v2w -= other.v2w;
	v3w -= other.v3w;
	v4w -= other.v4w;
}


void inline Matrix4x4::operator*=(const Matrix4x4& other)
{
	Vector4 x = Vector4(v1x, v2x, v3x, v4x);
	Vector4 y = Vector4(v1y, v2y, v3y, v4y);
	Vector4 z = Vector4(v1z, v2z, v3z, v4z);
	Vector4 w = Vector4(v1w, v2w, v3w, v4w);

	Vector4 one = Vector4(other.v1x, other.v1y, other.v1z, other.v1w);
	Vector4 two = Vector4(other.v2x, other.v2y, other.v2z, other.v2w);
	Vector4 three = Vector4(other.v3x, other.v3y, other.v3z, other.v3w);
	Vector4 four = Vector4(other.v4x, other.v4y, other.v4z, other.v4w);

	v1x = x.Dot(one);
	v2x = x.Dot(two);
	v3x = x.Dot(three);
	v4x = x.Dot(four);

	v1y = y.Dot(one);
	v2y = y.Dot(two);
	v3y = y.Dot(three);
	v4y = y.Dot(four);

	v1z = z.Dot(one);
	v2z = z.Dot(two);
	v3z = z.Dot(three);
	v4z = z.Dot(four);

	v1w = w.Dot(one);
	v2w = w.Dot(two);
	v3w = w.Dot(three);
	v4w = w.Dot(four);
}


#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far
