#pragma once

#include "PHOENIX/Math/Matrix.hpp"
#include "Vector.hpp"

class Quaternion : public Vector<4> {
public:
    Quaternion();
    Quaternion(const float *data);
    Quaternion(const Quaternion &other);
    ~Quaternion();

    Quaternion conjugate() const;
    Quaternion inverse() const;
    Quaternion &fromRotationMatrix(const Matrix3x3f &matrix);

    Matrix3x3f toRotationMatrix() const;
};