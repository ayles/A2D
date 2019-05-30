//
// Created by selya on 26.10.2018.
//

#ifndef A2D_MATRIX_HPP
#define A2D_MATRIX_HPP

#include <a2d/math/math_defines.hpp>
#include <a2d/math/vector.hpp>

namespace a2d {

template<int N, int M, class T>
class Matrix;

typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<3, 3, double> Matrix3d;
typedef Matrix<4, 4, double> Matrix4d;

// Matrix is column-major
// 0 4 8  12
// 1 5 9  13
// 2 6 10 14
// 3 7 11 15

template<class T>
class Matrix<4, 4, T> {
private:
    T data[16];

public:
    Matrix();

    Matrix(const Matrix<4, 4, T> &other);

    //
    // Non const methods
    //

    Matrix<4, 4, T> &Identity();
    Matrix<4, 4, T> &Set(const Matrix<4, 4, T> &other);
    Matrix<4, 4, T> &Inverse();
    Matrix<4, 4, T> &Transpose(T x, T y, T z);
    Matrix<4, 4, T> &Scale(T x, T y, T z);
    Matrix<4, 4, T> &Scale(const Vector<3, T> &scale);
    Matrix<4, 4, T> &Rotate(T ang, T x, T y, T z);
    Matrix<4, 4, T> &Rotate(T ang, const Vector<3, T> &axis);
    Matrix<4, 4, T> &Translate(T x, T y, T z);
    Matrix<4, 4, T> &Translate(const Vector<3, T> &translate);
    Matrix<4, 4, T> &SetOrtho2D(T left, T right, T bottom, T top);
    Matrix<4, 4, T> &operator=(const Matrix<4, 4, T> &other);

    T *operator[](int index);

    //
    // Const methods
    //

    Matrix<4, 4, T> operator*(const Matrix<4, 4, T> &other) const;
    const T *operator[](int index) const;

    Vector<4, T> &Transform(T x, T y, T z, T w, Vector<4, T> &dest) const;
    Vector<4, T> Transform(T x, T y, T z, T w) const;
    Vector<4, T> Transform(const Vector<4, T> &v) const;
    Vector<4, T> &Transform(const Vector<4, T> &v, Vector<4, T> &dest) const;
    Vector<4, T> Unproject(T x, T y, T z) const;
    Vector<4, T> Unproject(const Vector<3, T> &v) const;
    Vector<3, T> GetTranslation() const;

    float GetRotationZ() const;
};

} //namespace a2d

#endif //A2D_MATRIX_HPP
