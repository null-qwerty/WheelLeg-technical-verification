#pragma once

#include "Matrix.hpp"

template <int Dim> class Vector : public Matrix<Dim, 1> {
public:
    Vector();
    Vector(const float *data);
    Vector(const Vector<Dim> &other);
    ~Vector();

    float operator*(const Vector<Dim> &other) const;

    float &operator()(int index);
    const float &operator()(int index) const;

    float dot(const Vector<Dim> &other) const;
    Vector<Dim> cross(const Vector<Dim> &other) const;
    float magnitude() const;
    Vector<Dim> normalize() const;
};

template <int Dim>
Vector<Dim>::Vector()
    : Matrix<Dim, 1>()
{
}

template <int Dim>
Vector<Dim>::Vector(const float *data)
    : Matrix<Dim, 1>(data)
{
}

template <int Dim>
Vector<Dim>::Vector(const Vector<Dim> &other)
    : Matrix<Dim, 1>(other)
{
}

template <int Dim> Vector<Dim>::~Vector()
{
}

template <int Dim> float Vector<Dim>::operator*(const Vector<Dim> &other) const
{
    return dot(other);
}

template <int Dim> float &Vector<Dim>::operator()(int index)
{
    return this->data[index];
}

template <int Dim> const float &Vector<Dim>::operator()(int index) const
{
    return this->data[index];
}

template <int Dim> float Vector<Dim>::dot(const Vector<Dim> &other) const
{
    float result;
    arm_dot_prod_f32(this->data, other.data, Dim, &result);
    return result;
}

template <int Dim>
Vector<Dim> Vector<Dim>::cross(const Vector<Dim> &other) const
{
    Vector<Dim> result;
    arm_cross_f32(this->data, other.data, result.data);
    return result;
}

template <int Dim> float Vector<Dim>::magnitude() const
{
    float result;
    arm_power_f32(this->data, Dim, &result);
    return sqrtf(result);
}

template <int Dim> Vector<Dim> Vector<Dim>::normalize() const
{
    Vector<Dim> result;
    arm_scale_f32(this->data, 1.0f / this->magnitude(), result.data, Dim);
    return result;
}

using Vector2f = Vector<2>;
using Vector3f = Vector<3>;
using Vector4f = Vector<4>;
using Vector5f = Vector<5>;
using Vector6f = Vector<6>;
using Vector7f = Vector<7>;
using Vector8f = Vector<8>;
using Vector9f = Vector<9>;