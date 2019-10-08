#pragma once
#include <array>

using std::array;
template <typename T, unsigned int size>
struct VectorBase
{
public:
    array<T, size> vector;

public:
    VectorBase();
    VectorBase(T values[size]);

    T Dot(const VectorBase <T, size>& other);
    float Magnitude();
    float SqrMagnitude();
    void Normalize();
    void Absolute();

    VectorBase<T, size> operator+(const VectorBase<T, size>& other);
    VectorBase<T, size> operator-(const VectorBase<T, size>& other);
    VectorBase<T, size> operator*(const VectorBase<T, size>& other);
    VectorBase<T, size> operator/(const VectorBase<T, size>& other);

    void operator+=(const VectorBase<T, size>& other);
    void operator-=(const VectorBase<T, size>& other);
    void operator*=(const VectorBase<T, size>& other);
    void operator/=(const VectorBase<T, size>& other);

    VectorBase<T, size> operator*(const T& other);
    VectorBase<T, size> operator/(const T& other);

    void operator*=(const T& other);
    void operator/=(const T& other);
};

template<typename T, unsigned int size>
inline VectorBase<T, size>::VectorBase()
{
    vector.fill(0);
}

template<typename T, unsigned int size>
inline VectorBase<T, size>::VectorBase(T values[size])
{
    for (int i = 0; i < size; i++)
        vector[i] = values[size];
}

template<typename T, unsigned int size>
inline T VectorBase<T, size>::Dot(const VectorBase<T, size>& other)
{
    T output = 0;
    for (int i = 0; i < size; i++)
        output += vector[i] * other[i];
    return output;
}

template<typename T, unsigned int size>
inline float VectorBase<T, size>::Magnitude()
{   
    return sqrt<T>(SqrMagnitude());
}

template<typename T, unsigned int size>
inline float VectorBase<T, size>::SqrMagnitude()
{
    T output = 0;
    for (int i = 0; i < size; i++)
        output += vector[i] * vector[i];
    return output;
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::Normalize()
{
    T magnitude = Magnitude();
    *this /= magnitude;
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::Absolute()
{
    for (int i = 0; i < size; i++)
        abs(vector[i]);
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator+(const VectorBase<T, size>& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] + other[i];
    return output;
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator-(const VectorBase<T, size>& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] - other[i];
    return output;
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator*(const VectorBase<T, size>& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] * other[i];
    return output;
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator/(const VectorBase<T, size>& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] / other[i];
    return output;
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator+=(const VectorBase<T, size>& other)
{
    for (int i = 0; i < size; i++)
        vector[i] += other[i];
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator-=(const VectorBase<T, size>& other)
{
    for (int i = 0; i < size; i++)
        vector[i] -= other[i];
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator*=(const VectorBase<T, size>& other)
{
    for (int i = 0; i < size; i++)
        vector[i] *= other[i];
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator/=(const VectorBase<T, size>& other)
{
    for (int i = 0; i < size; i++)
       vector[i] /= other[i];
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator*(const T& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] * other;
    return output;
}

template<typename T, unsigned int size>
inline VectorBase<T, size> VectorBase<T, size>::operator/(const T& other)
{
    VectorBase<T, size> output;
    for (int i = 0; i < size; i++)
        output[i] = vector[i] / other;
    return output;
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator*=(const T& other)
{
    for (int i = 0; i < size; i++)
       vector[i] *= other;
}

template<typename T, unsigned int size>
inline void VectorBase<T, size>::operator/=(const T& other)
{
    for (int i = 0; i < size; i++)
        vector[i] /= other;
}
