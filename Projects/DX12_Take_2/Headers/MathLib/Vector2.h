#pragma once
#include "MathConstants.h"

struct Vector2
{
public:
	Vector2() = default;
	Vector2(float _x, float _y);

	float Dot(const Vector2& other);
	float Magnitude();
	void Normalize();
	void Absolute();
    Vector2 GetNormalized();

	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2 operator*(const Vector2& other);
	Vector2 operator/(const Vector2& other);

	void operator+=(const Vector2& other);
	void operator-=(const Vector2& other);
	void operator*=(const Vector2& other);
	void operator/=(const Vector2& other);

	Vector2 operator*(const float& other);
	Vector2 operator/(const float& other);

	void operator*=(const float& other);
	void operator/=(const float& other);
public:
	float x = 0;
	float y = 0;
};

inline Vector2::Vector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

float inline Vector2::Dot(const Vector2& other)
{
	return (x * other.x) + (y * other.y);
}

float inline Vector2::Magnitude()
{
	return sqrtf((x * x) + (y * y));
}

void inline Vector2::Normalize()
{
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
}

void inline Vector2::Absolute()
{
	x = abs(x);
	y = abs(y);
}

inline Vector2 Vector2::GetNormalized()
{
    Vector2 normalized = *this;
    normalized.Normalize();
    return normalized;
}

Vector2 inline Vector2::operator+(const Vector2& other)
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 inline Vector2::operator-(const Vector2& other)
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 inline Vector2::operator*(const Vector2& other)
{
	return Vector2(x * other.x, y * other.y);
}

Vector2 inline Vector2::operator/(const Vector2& other)
{
	return Vector2(x / other.x, y / other.y);
}

void inline Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
}

void inline Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
}

void inline Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;
}

void inline Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;
}

Vector2 inline Vector2::operator*(const float& other)
{
	return Vector2(x * other, y * other);
}

Vector2 inline Vector2::operator/(const float& other)
{
	return Vector2(x / other, y / other);
}

void inline Vector2::operator*=(const float& other)
{
	x *= other;
	y *= other;
}

void inline Vector2::operator/=(const float& other)
{
	x /= other;
	y /= other;
}
