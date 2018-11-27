//
// Created by selya on 26.10.2018.
//

#ifndef A2D_MATRIX_H
#define A2D_MATRIX_H

#include <a2d/math/vector.h>

namespace a2d {

template<int N, int M, class T, class F = void>
class Matrix;

typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<3, 3, double> Matrix3d;
typedef Matrix<4, 4, double> Matrix4d;

template<class T>
class Matrix<4, 4, T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
private:
    T data[16];

public:
    Matrix();
    Matrix(const Matrix<4, 4, T> &matrix);

    //
    // Const methods
    //

    Vector<4, T> &Transform(float x, float y, float z, float w, Vector<4, T> &dest) const;
    Vector<4, T> &Transform(Vector<4, T> &v) const;
    Vector<4, T> &Transform(const Vector<4, T> &v, Vector<4, T> &dest) const;

    //
    // Non const methods
    //

    Matrix<4, 4, T> &Identity();

    Matrix<4, 4, T> &Scale(T x, T y, T z);
    Matrix<4, 4, T> &Scale(const Vector<3, T> &scale);

    Matrix<4, 4, T> &Rotate(T ang, T x, T y, T z);
    Matrix<4, 4, T> &Rotate(T ang, const Vector<3, T> &axis);

    Matrix<4, 4, T> &Translate(T x, T y, T z);
    Matrix<4, 4, T> &Translate(const Vector<3, T> &translate);

    Matrix<4, 4, T> &Invert();

    Matrix<4, 4, T> &SetOrtho2D(T left, T right, T bottom, T top);

    Matrix<4, 4, T> &operator=(const Matrix<4, 4, T> &other);
    T *operator[](int index);

    //
    // Const methods
    //

    Matrix<4, 4, T> operator*(const Matrix<4, 4, T> &other) const;
    const T *operator[](int index) const;
};

} //namespace a2d

#endif //A2D_MATRIX_H
