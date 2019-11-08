#pragma once
#include "MathConstants.h"

struct Vector2
{
public:
    union
    {
        float data[2];
        struct { float x, y; };
        struct { float r, g; };
        struct { float u, v; };
    };

public:
	Vector2() = default;

    Vector2(float _x, float _y) : x(_x), y(_y)
    {
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 operator*(const Vector2& other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
    Vector2 operator/(const Vector2& other) const
    {
        return Vector2(x / other.x, y / other.y);
    }
    Vector2 operator*(const float& other) const
    {
        return Vector2(x * other, y * other);
    }
    Vector2 operator/(const float& other) const
    {
        return Vector2(x / other, y / other);
    }

    void operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator*=(const Vector2& other)
    {
        x *= other.x;
        y *= other.y;
    }
    void operator/=(const Vector2& other)
    {
        x /= other.x;
        y /= other.y;
    }
    void operator*=(const float& other)
    {
        x *= other;
        y *= other;
    }
    void operator/=(const float& other)
    {
        x /= other;
        y /= other;
    }

    bool operator==(const Vector2& other) const
    {
        return (x == other.x && y == other.y);
    }

public:
    float Dot(const Vector2& other) const
    {
        return (x * other.x) + (y * other.y);
    }
    float Magnitude() const
    {
        return sqrtf(MagnitudeSquared());
    }
    float MagnitudeSquared() const
    {
        return (x * x) + (y * y);
    }
    void Normalize()
    {
        float magnitude = Magnitude();
        x /= magnitude;
        y /= magnitude;
    }
    void Absolute()
    {
        x = abs(x);
        y = abs(y);
    }
    Vector2 GetNormalized() const
    {
        Vector2 normalized = *this;
        normalized.Normalize();
        return normalized;
    }
};

