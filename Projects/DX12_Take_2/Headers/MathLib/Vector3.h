#pragma once
#include "MathConstants.h"
#include "Vector2.h"

struct Vector3
{
public:
	Vector3();
	Vector3(float _x, float _y, float _z);
	Vector3(Vector2 _xy, float _z = 0);

	float Dot(const Vector3& other);
	Vector3 Cross(const Vector3& other);
	float Magnitude();
	void Normalize();
	void Absolute();

	Vector3 operator+(const Vector3& other);
	Vector3 operator-(const Vector3& other);
	Vector3 operator*(const Vector3& other);
	Vector3 operator/(const Vector3& other);

	void operator+=(const Vector3& other);
	void operator-=(const Vector3& other);
	void operator*=(const Vector3& other);
	void operator/=(const Vector3& other);

	Vector3 operator*(const float& other);
	Vector3 operator/(const float& other);

	void operator*=(const float& other);
	void operator/=(const float& other);
public:
	float x;
	float y;
	float z;
};


inline Vector3::Vector3() :
	x(0),
	y(0),
	z(0)
{
}

inline Vector3::Vector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

inline Vector3::Vector3(Vector2 _xy, float _z) : Vector3(_xy.x, _xy.y, z)
{
}


float inline Vector3::Dot(const Vector3& other)
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 inline Vector3::Cross(const Vector3& other)
{
	float x = (y * other.z) - (other.y * z);
	float y = (z* other.x) - (other.z * x);
	float z = (x * other.y) - (other.x * y);
	return Vector3(x, y, z);
}

float inline Vector3::Magnitude()
{
	return sqrtf((x * x) + (y * y) + (z*z));
}

void inline Vector3::Normalize()
{
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

void inline Vector3::Absolute()
{
	x = abs(x);
	y = abs(y);
	z = abs(z);
}


Vector3 inline Vector3::operator+(const Vector3& other)
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 inline Vector3::operator-(const Vector3& other)
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 inline Vector3::operator*(const Vector3& other)
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 inline Vector3::operator/(const Vector3& other)
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

void inline Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void inline Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void inline Vector3::operator*=(const Vector3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

void inline Vector3::operator/=(const Vector3& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
}

Vector3 inline Vector3::operator*(const float& other)
{
	return Vector3(x * other, y * other, z * other);
}

Vector3 inline Vector3::operator/(const float& other)
{
	return Vector3(x / other, y / other, z / other);
}

void inline Vector3::operator*=(const float& other)
{
	x *= other;
	y *= other;
	z *= other;
}

void inline Vector3::operator/=(const float& other)
{
	x /= other;
	y /= other;
	z /= other;
}
