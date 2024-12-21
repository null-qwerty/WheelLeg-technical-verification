#pragma once

#include "arm_math.h"
#include "matrix_functions.h"

template <int Rows, int Cols> class Matrix {
public:
    Matrix();
    Matrix(const float *data);
    Matrix(const Matrix<Rows, Cols> &other);
    ~Matrix();

    Matrix<Rows, Cols> &operator=(const Matrix<Rows, Cols> &other);
    Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols> &other) const;
    Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols> &other) const;
    Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &other) const;
    Matrix<Rows, Cols> operator*(const float &scalar) const;
    Matrix<Rows, Cols> operator/(const float &scalar) const;
    Matrix<Rows, Cols> operator-() const;
    Matrix<Rows, Cols> operator+() const;
    Matrix<Rows, Cols> &operator~() const;
    Matrix<Rows, Cols> &operator+=(const Matrix<Rows, Cols> &other);
    Matrix<Rows, Cols> &operator-=(const Matrix<Rows, Cols> &other);
    Matrix<Rows, Cols> &operator*=(const Matrix<Rows, Cols> &other);
    Matrix<Rows, Cols> &operator*=(const float &scalar);
    Matrix<Rows, Cols> &operator/=(const float &scalar);

    Matrix<Rows, Cols> &operator<<(float data) const;

    float &operator()(int row, int col);
    const float &operator()(int row, int col) const;

    Matrix<Rows, Cols> transpose() const;
    Matrix<Rows, Cols> inverse() const;

    static Matrix<Rows, Cols> identity();
    static Matrix<Rows, Cols> zero();

    static Matrix<Rows, Cols> fromArray(const float *data);
    void toArray(float *data) const;

    static Matrix<Rows, Cols> fromArray(const float *data, int rows, int cols);
    void toArray(float *data, int rows, int cols) const;

protected:
    arm_matrix_instance_f32 matrix;
    float data[Rows * Cols];
};

template <int Rows, int Cols> Matrix<Rows, Cols>::Matrix()
{
    arm_mat_init_f32(&matrix, Rows, Cols, data);
}

template <int Rows, int Cols> Matrix<Rows, Cols>::Matrix(const float *data)
{
    arm_mat_init_f32(&matrix, Rows, Cols, this->data);
    arm_copy_f32(data, this->data, Rows * Cols);
}

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const Matrix<Rows, Cols> &other)
{
    arm_mat_init_f32(&matrix, Rows, Cols, data);
    arm_copy_f32(other.data, data, Rows * Cols);
}

template <int Rows, int Cols> Matrix<Rows, Cols>::~Matrix()
{
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &
Matrix<Rows, Cols>::operator=(const Matrix<Rows, Cols> &other)
{
    arm_copy_f32(other.data, data, Rows * Cols);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols>
Matrix<Rows, Cols>::operator+(const Matrix<Rows, Cols> &other) const
{
    Matrix<Rows, Cols> result;
    arm_mat_add_f32(&matrix, &other.matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols>
Matrix<Rows, Cols>::operator-(const Matrix<Rows, Cols> &other) const
{
    Matrix<Rows, Cols> result;
    arm_mat_sub_f32(&matrix, &other.matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols>
Matrix<Rows, Cols>::operator*(const Matrix<Rows, Cols> &other) const
{
    Matrix<Rows, Cols> result;
    arm_mat_mult_f32(&matrix, &other.matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator*(const float &scalar) const
{
    Matrix<Rows, Cols> result;
    arm_mat_scale_f32(&matrix, scalar, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator/(const float &scalar) const
{
    Matrix<Rows, Cols> result;
    arm_mat_scale_f32(&matrix, 1.0f / scalar, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator-() const
{
    Matrix<Rows, Cols> result;
    arm_negate_f32(&matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator+() const
{
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator~() const
{
    return this->inverse();
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &
Matrix<Rows, Cols>::operator+=(const Matrix<Rows, Cols> &other)
{
    arm_mat_add_f32(&matrix, &other.matrix, &matrix);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &
Matrix<Rows, Cols>::operator-=(const Matrix<Rows, Cols> &other)
{
    arm_mat_sub_f32(&matrix, &other.matrix, &matrix);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &
Matrix<Rows, Cols>::operator*=(const Matrix<Rows, Cols> &other)
{
    arm_mat_mult_f32(&matrix, &other.matrix, &matrix);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator*=(const float &scalar)
{
    arm_mat_scale_f32(&matrix, scalar, &matrix);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator/=(const float &scalar)
{
    arm_mat_scale_f32(&matrix, 1.0f / scalar, &matrix);
    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator<<(float data) const
{
    static int index = 0;
    if (index < Rows * Cols)
        this->data[index++] = data;
    return *this;
}

template <int Rows, int Cols>
float &Matrix<Rows, Cols>::operator()(int row, int col)
{
    return data[row * Cols + col];
}

template <int Rows, int Cols>
const float &Matrix<Rows, Cols>::operator()(int row, int col) const
{
    return data[row * Cols + col];
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::transpose() const
{
    Matrix<Rows, Cols> result;
    arm_mat_trans_f32(&matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::inverse() const
{
    Matrix<Rows, Cols> result;
    arm_mat_inverse_f32(&matrix, &result.matrix);
    return result;
}

template <int Rows, int Cols> Matrix<Rows, Cols> Matrix<Rows, Cols>::identity()
{
    Matrix<Rows, Cols> result;
    arm_mat_identity_f32(&result.matrix);
    return result;
}

template <int Rows, int Cols> Matrix<Rows, Cols> Matrix<Rows, Cols>::zero()
{
    Matrix<Rows, Cols> result;
    arm_mat_zero_f32(&result.matrix);
    return result;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::fromArray(const float *data)
{
    Matrix<Rows, Cols> result;
    arm_copy_f32(data, result.data, Rows * Cols);
    return result;
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::toArray(float *data) const
{
    arm_copy_f32(this->data, data, Rows * Cols);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::fromArray(const float *data, int rows,
                                                 int cols)
{
    Matrix<Rows, Cols> result;
    arm_copy_f32(data, result.data, rows * cols);
    return result;
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::toArray(float *data, int rows, int cols) const
{
    arm_copy_f32(this->data, data, rows * cols);
}

using Matrix2x2f = Matrix<2, 2>;
using Matrix3x3f = Matrix<3, 3>;
using Matrix4x4f = Matrix<4, 4>;
using Matrix5x5f = Matrix<5, 5>;
using Matrix6x6f = Matrix<6, 6>;
using Matrix7x7f = Matrix<7, 7>;
using Matrix8x8f = Matrix<8, 8>;
using Matrix9x9f = Matrix<9, 9>;