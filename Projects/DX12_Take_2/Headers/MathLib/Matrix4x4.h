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

	Vector3 GetPosition() { return Vector3(vX.w, vY.w, vZ.w); }
	Vector3 GetScale() { return Vector3(vX.x, vY.y, vZ.z); }
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
	vX = { temp.vX.x, temp.vY.x, temp.vZ.x, temp.vW.x };
	vY = { temp.vX.y, temp.vY.y, temp.vZ.y, temp.vW.y };
	vZ = { temp.vX.z, temp.vY.z, temp.vZ.z, temp.vW.z };
	vW = { temp.vX.w, temp.vY.w, temp.vZ.w, temp.vW.w };
}

inline void Matrix4x4::SetOrtho(float width, float height, float near, float far)
{
	//The following docs are in row major, this matrix uses column major
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh Left handed, posZIn = true
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh Right handed, posZIn = false
	Identity();
#if USE_USE_LEFT_HANDED_MATRIXS
		vX.x = 2 / width;
		vY.y = 2 / height;
		vZ.z = 1 / (far - near);
		vZ.w = -near / (far - near);
#else
		vX.x = 2 / width;
		vY.y = 2 / height;
		vZ.z = 1 / (near - far);
		vZ.w = near / (near - far);
#endif
}

inline void Matrix4x4::SetPerspective(float fovAngleY, float aspectRatio, float near, float far)
{
	//The following docs are in row major, this matrix uses column major
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh Left Handed, posZIn = true
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh Right handed, posZIn = false
	float yScale = 1 / tanf(fovAngleY * 3.14f / 180.0f * 0.5f);
	float xScale = yScale / aspectRatio;

	Identity();
#if USE_USE_LEFT_HANDED_MATRIXS
		vX.x = xScale;
		vY.y = yScale;
		vZ.z = far / (far - near);
		vZ.w = (-near * far) / (far - near);
		vW.z = 1;
		vW.w = 0;
#else
		vX.x = xScale;
		vY.y = yScale;
		vZ.z = far / (near - far);
		vZ.w = (near * far) / (near - far);
		vW.z = -1;
		vW.w = 0;
#endif
}

inline void Matrix4x4::SetPosition(Vector3 position)
{
	vX.w = position.x;
	vY.w = position.y;
	vZ.w = position.z;
}

inline void Matrix4x4::Translate(Vector3 position)
{
	vX.w += position.x;
	vY.w += position.y;
	vZ.w += position.z;
}

inline void Matrix4x4::RotateX(float degrees)
{
	Matrix4x4 rotMatrix;
	float radians = degrees / 180.f * PI;

	float sinAngle = sinf(radians);
	float cosAngle = cosf(radians);

	rotMatrix.vY.y = cosAngle;
	rotMatrix.vY.z = sinAngle;
	rotMatrix.vZ.y = -sinAngle;
	rotMatrix.vZ.z = cosAngle;

	*this *= rotMatrix;
}

inline void Matrix4x4::RotateY(float degrees)
{
	Matrix4x4 rotMatrix;
	float radians = degrees / 180.f * PI;

	float sinAngle = sinf(radians);
	float cosAngle = cosf(radians);

	rotMatrix.vX.x = cosAngle;
	rotMatrix.vX.z = sinAngle;
	rotMatrix.vZ.x = -sinAngle;
	rotMatrix.vZ.z = cosAngle;

	*this *= rotMatrix;
}

inline void Matrix4x4::RotateZ(float degrees)
{
	Matrix4x4 rotMatrix;
	float radians = degrees / 180.f * PI;

	float sinAngle = sinf(radians);
	float cosAngle = cosf(radians);

	rotMatrix.vX.x = cosAngle;
	rotMatrix.vX.y = -sinAngle;
	rotMatrix.vY.x = sinAngle;
	rotMatrix.vY.y = cosAngle;

	*this *= rotMatrix;
}

inline void Matrix4x4::SetScale(Vector3 scale)
{
	vX.x = scale.x;
	vY.y = scale.y;
	vZ.z = scale.z;
}

inline void Matrix4x4::Scale(Vector3 scale)
{
	vX.x += scale.x;
	vY.y += scale.y;
	vZ.z += scale.z;
}

inline Vector3 Matrix4x4::GetEulerAngles()
{
	float radToDeg = 180 / PI;
	if (vY.z > 1.0f)
	{
		float x = PI / 2;
		float y = atan2f(vX.y, vX.x);
		float z = 0.0f;
		return Vector3(x, y, z) * radToDeg;
	}
	else if (vY.z < -1.0f)
	{
		float x = PI / 2;
		float y = -atan2f(vX.y, vX.x);
		float z = 0.0f;
		return Vector3(x, y, z) * radToDeg;
	}
	else
	{
		float x = asinf(vY.z);
		float y = atan2f(-vX.z, vZ.z);
		float z = atan2f(-vY.x, vY.y);
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

	output.vX.x = vX.Dot(mat.vX);
	output.vX.y = vX.Dot(mat.vY);
	output.vX.z = vX.Dot(mat.vZ);
	output.vX.w = vX.Dot(mat.vW);

	output.vY.x = vY.Dot(mat.vX);
	output.vY.y = vY.Dot(mat.vY);
	output.vY.z = vY.Dot(mat.vZ);
	output.vY.w = vY.Dot(mat.vW);

	output.vZ.x = vZ.Dot(mat.vX);
	output.vZ.y = vZ.Dot(mat.vY);
	output.vZ.z = vZ.Dot(mat.vZ);
	output.vZ.w = vZ.Dot(mat.vW);

	output.vW.x = vW.Dot(mat.vX);
	output.vW.y = vW.Dot(mat.vY);
	output.vW.z = vW.Dot(mat.vZ);
	output.vW.w = vW.Dot(mat.vW);

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
	
    vX.x = copy.vX.Dot(mat.vX);
    vX.y = copy.vX.Dot(mat.vY);
    vX.z = copy.vX.Dot(mat.vZ);
    vX.w = copy.vX.Dot(mat.vW);

    vY.x = copy.vY.Dot(mat.vX);
    vY.y = copy.vY.Dot(mat.vY);
    vY.z = copy.vY.Dot(mat.vZ);
    vY.w = copy.vY.Dot(mat.vW);

    vZ.x = copy.vZ.Dot(mat.vX);
    vZ.y = copy.vZ.Dot(mat.vY);
    vZ.z = copy.vZ.Dot(mat.vZ);
    vZ.w = copy.vZ.Dot(mat.vW);

    vW.x = copy.vW.Dot(mat.vX);
    vW.y = copy.vW.Dot(mat.vY);
    vW.z = copy.vW.Dot(mat.vZ);
    vW.w = copy.vW.Dot(mat.vW);
}


#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far
