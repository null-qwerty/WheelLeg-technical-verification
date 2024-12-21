#include "PHOENIX/Math/Quaternion.hpp"
#include "PHOENIX/Math/Matrix.hpp"

Quaternion::Quaternion()
    : Vector<4>()
{
}

Quaternion::Quaternion(const float *data)
    : Vector<4>(data)
{
}

Quaternion::Quaternion(const Quaternion &other)
    : Vector<4>(other)
{
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::conjugate() const
{
    Quaternion result;
    result(0) = this->data[0];
    result(1) = -this->data[1];
    result(2) = -this->data[2];
    result(3) = -this->data[3];
    return result;
}

Quaternion Quaternion::inverse() const
{
    Quaternion result;
    float magnitude = this->magnitude();
    result(0) = this->data[0] / magnitude;
    result(1) = -this->data[1] / magnitude;
    result(2) = -this->data[2] / magnitude;
    result(3) = -this->data[3] / magnitude;
    return result;
}

Quaternion &Quaternion::fromRotationMatrix(const Matrix3x3f &matrix)
{
    float trace = matrix(0, 0) + matrix(1, 1) + matrix(2, 2);
    if (trace > 0.0f) {
        float s = 0.5f / sqrtf(trace + 1.0f);
        this->data[0] = 0.25f / s;
        this->data[1] = (matrix(2, 1) - matrix(1, 2)) * s;
        this->data[2] = (matrix(0, 2) - matrix(2, 0)) * s;
        this->data[3] = (matrix(1, 0) - matrix(0, 1)) * s;
    } else {
        if (matrix(0, 0) > matrix(1, 1) && matrix(0, 0) > matrix(2, 2)) {
            float s =
                2.0f * sqrtf(1.0f + matrix(0, 0) - matrix(1, 1) - matrix(2, 2));
            this->data[0] = (matrix(2, 1) - matrix(1, 2)) / s;
            this->data[1] = 0.25f * s;
            this->data[2] = (matrix(0, 1) + matrix(1, 0)) / s;
            this->data[3] = (matrix(0, 2) + matrix(2, 0)) / s;
        } else if (matrix(1, 1) > matrix(2, 2)) {
            float s =
                2.0f * sqrtf(1.0f + matrix(1, 1) - matrix(0, 0) - matrix(2, 2));
            this->data[0] = (matrix(0, 2) - matrix(2, 0)) / s;
            this->data[1] = (matrix(0, 1) + matrix(1, 0)) / s;
            this->data[2] = 0.25f * s;
            this->data[3] = (matrix(1, 2) + matrix(2, 1)) / s;
        } else {
            float s =
                2.0f * sqrtf(1.0f + matrix(2, 2) - matrix(0, 0) - matrix(1, 1));
            this->data[0] = (matrix(1, 0) - matrix(0, 1)) / s;
            this->data[1] = (matrix(0, 2) + matrix(2, 0)) / s;
            this->data[2] = (matrix(1, 2) + matrix(2, 1)) / s;
            this->data[3] = 0.25f * s;
        }
    }
    return *this;
}

Matrix3x3f Quaternion::toRotationMatrix() const
{
    Matrix3x3f result;

    float xx = this->data[1] * this->data[1];
    float xy = this->data[1] * this->data[2];
    float xz = this->data[1] * this->data[3];
    float xw = this->data[1] * this->data[0];
    float yy = this->data[2] * this->data[2];
    float yz = this->data[2] * this->data[3];
    float yw = this->data[2] * this->data[0];
    float zz = this->data[3] * this->data[3];
    float zw = this->data[3] * this->data[0];

    result(0, 0) = 1.0f - 2.0f * (yy + zz);
    result(0, 1) = 2.0f * (xy - zw);
    result(0, 2) = 2.0f * (xz + yw);
    result(1, 0) = 2.0f * (xy + zw);
    result(1, 1) = 1.0f - 2.0f * (xx + zz);
    result(1, 2) = 2.0f * (yz - xw);
    result(2, 0) = 2.0f * (xz - yw);
    result(2, 1) = 2.0f * (yz + xw);
    result(2, 2) = 1.0f - 2.0f * (xx + yy);

    return result;
}