#pragma once
#include "MathConstants.h"
#include "Vector3.h"

struct Vector4
{
public:
	Vector4() = default;
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(Vector3 _xyz, float _w = 0);

	float Dot(const Vector4& other);
	float Magnitude();
	float MagnitudeSquared();
	void Normalize();
	void Absolute();

	Vector4 operator+(const Vector4& other);
	Vector4 operator-(const Vector4& other);
	Vector4 operator*(const Vector4& other);
	Vector4 operator/(const Vector4& other);

	void operator+=(const Vector4& other);
	void operator-=(const Vector4& other);
	void operator*=(const Vector4& other);
	void operator/=(const Vector4& other);

	Vector4 operator*(const float& other);
	Vector4 operator/(const float& other);

	void operator*=(const float& other);
	void operator/=(const float& other);


public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

inline Vector4::Vector4(float _x, float _y, float _z, float _w) :
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
}

inline Vector4::Vector4(Vector3 _xyz, float _w) : Vector4(_xyz.x, _xyz.y, _xyz.z, _w)
{
}

float inline Vector4::Dot(const Vector4& other)
{
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}


float inline Vector4::Magnitude()
{
	return sqrtf(MagnitudeSquared());
}

inline float Vector4::MagnitudeSquared()
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}


void inline Vector4::Normalize()
{
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	w /= magnitude;
}

void inline Vector4::Absolute()
{
	x = abs(x);
	y = abs(y);
	z = abs(z);
	w = abs(w);
}

Vector4 inline Vector4::operator+(const Vector4& other)
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 inline Vector4::operator-(const Vector4& other)
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 inline Vector4::operator*(const Vector4& other)
{
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4 inline Vector4::operator/(const Vector4& other)
{
	return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

void inline Vector4::operator+=(const Vector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
}

void inline Vector4::operator-=(const Vector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
}

void inline Vector4::operator*=(const Vector4& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
}

void inline Vector4::operator/=(const Vector4& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
}

Vector4 inline Vector4::operator*(const float& other)
{
	return Vector4(x * other, y * other, z * other, w * other);
}

Vector4 inline Vector4::operator/(const float& other)
{
	return Vector4(x / other, y / other, z / other, w / other);
}

void inline Vector4::operator*=(const float& other)
{
	x *= other;
	y *= other;
	z *= other;
	w *= other;
}

void inline Vector4::operator/=(const float& other)
{
	x /= other;
	y /= other;
	z /= other;
	w /= other;
}

