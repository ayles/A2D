//
// Created by selya on 26.10.2018.
//

#include <a2d/math/matrix.h>

#include <cmath>
#include <a2d/core/engine.h>

#define IF_FLOATING_POINT typename std::enable_if<std::is_floating_point<T>::value>::type
#define IF_INTEGRAL typename std::enable_if<std::is_integral<T>::value>::type

// Matrix template specialization for Mat4f
template<class T>
a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Matrix() : data{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } {}

template<class T>
a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Matrix(const a2d::Matrix<4, 4, T> &other) {
    data[0] = other.data[0];
    data[1] = other.data[1];
    data[2] = other.data[2];
    data[3] = other.data[3];

    data[4] = other.data[4];
    data[5] = other.data[5];
    data[6] = other.data[6];
    data[7] = other.data[7];

    data[8] = other.data[8];
    data[9] = other.data[9];
    data[10] = other.data[10];
    data[11] = other.data[11];

    data[12] = other.data[12];
    data[13] = other.data[13];
    data[14] = other.data[14];
    data[15] = other.data[15];
}

//
// Non const methods
//

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Identity() {
    data[0] = 1;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    data[4] = 0;
    data[5] = 1;
    data[6] = 0;
    data[7] = 0;

    data[8] = 0;
    data[9] = 0;
    data[10] = 1;
    data[11] = 0;

    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 1;
    return *this;
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Scale(const a2d::Vector<3, T> &scale) {
    return a2d::Matrix<4, 4, T>::Scale(scale.x, scale.y, scale.z);
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Scale(T x, T y, T z) {

    this->data[0] *= x;
    this->data[1] *= x;
    this->data[2] *= x;
    this->data[3] *= x;

    this->data[4] *= y;
    this->data[5] *= y;
    this->data[6] *= y;
    this->data[7] *= y;

    this->data[8] *= z;
    this->data[9] *= z;
    this->data[10] *= z;
    this->data[11] *= z;

    return *this;
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Rotate(T ang, const Vector<3, T> &axis) {
    return a2d::Matrix<4, 4, T>::Rotate(ang, axis.x, axis.y, axis.z);
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Rotate(T ang, T x, T y, T z) {
    T sin = std::sin(ang);
    T cos;

    // TODO move to function
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double PI2 = PI * 2.0;
    static constexpr double PIHalf = PI * 0.5;

    double cos_v = std::sqrt(1.0 - sin * sin),
            a = ang + PIHalf,
            b = a - int(a / PI2) * PI2;

    if (b < 0.0)
        b = PI2 + b;
    cos = (T)(b >= PI ? -cos_v : cos_v);
    //

    T C = 1.0f - cos;

    T xx = x * x, xy = x * y, xz = x * z;
    T yy = y * y, yz = y * z;
    T zz = z * z;

    T rm00 = xx * C + cos;
    T rm01 = xy * C + z * sin;
    T rm02 = xz * C - y * sin;
    T rm10 = xy * C - z * sin;
    T rm11 = yy * C + cos;
    T rm12 = yz * C + x * sin;
    T rm20 = xz * C + y * sin;
    T rm21 = yz * C - x * sin;
    T rm22 = zz * C + cos;

    T nm00 = this->data[0] * rm00 + this->data[4] * rm01 + this->data[8] * rm02;
    T nm01 = this->data[1] * rm00 + this->data[5] * rm01 + this->data[9] * rm02;
    T nm02 = this->data[2] * rm00 + this->data[6] * rm01 + this->data[10] * rm02;
    T nm03 = this->data[3] * rm00 + this->data[7] * rm01 + this->data[11] * rm02;
    T nm10 = this->data[0] * rm10 + this->data[4] * rm11 + this->data[8] * rm12;
    T nm11 = this->data[1] * rm10 + this->data[5] * rm11 + this->data[9] * rm12;
    T nm12 = this->data[2] * rm10 + this->data[6] * rm11 + this->data[10] * rm12;
    T nm13 = this->data[3] * rm10 + this->data[7] * rm11 + this->data[11] * rm12;

    this->data[8] = this->data[0] * rm20 + this->data[4] * rm21 + this->data[8] * rm22;
    this->data[9] = this->data[1] * rm20 + this->data[5] * rm21 + this->data[9] * rm22;
    this->data[10] = this->data[2] * rm20 + this->data[6] * rm21 + this->data[10] * rm22;
    this->data[11] = this->data[3] * rm20 + this->data[7] * rm21 + this->data[11] * rm22;

    this->data[0] = nm00;
    this->data[1] = nm01;
    this->data[2] = nm02;
    this->data[3] = nm03;
    this->data[4] = nm10;
    this->data[5] = nm11;
    this->data[6] = nm12;
    this->data[7] = nm13;

    return *this;
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Translate(const Vector<3, T> &translate) {
    return a2d::Matrix<4, 4, T>::Translate(translate.x, translate.y, translate.z);
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Translate(T x, T y, T z) {
    this->data[12] = data[0] * x + data[4] * y + data[8] * z + data[12];
    this->data[13] = data[1] * x + data[5] * y + data[9] * z + data[13];
    this->data[14] = data[2] * x + data[6] * y + data[10] * z + data[14];
    this->data[15] = data[3] * x + data[7] * y + data[11] * z + data[15];

    return *this;
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::SetOrtho2D(
        T left, T right, T bottom, T top
) {
    data[0] = 2.0f / (right - left);
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    data[4] = 0;
    data[5] = 2.0f / (top - bottom);
    data[6] = 0;
    data[7] = 0;

    data[8] = 0;
    data[9] = 0;
    data[10] = -1.0f;
    data[11] = 0;

    data[12] = (right + left) / (left - right);
    data[13] = (top + bottom) / (bottom - top);
    data[14] = 0;
    data[15] = 1;
    return *this;
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::operator=(const Matrix<4, 4, T> &other) {
    data[0] = other.data[0];
    data[1] = other.data[1];
    data[2] = other.data[2];
    data[3] = other.data[3];

    data[4] = other.data[4];
    data[5] = other.data[5];
    data[6] = other.data[6];
    data[7] = other.data[7];

    data[8] = other.data[8];
    data[9] = other.data[9];
    data[10] = other.data[10];
    data[11] = other.data[11];

    data[12] = other.data[12];
    data[13] = other.data[13];
    data[14] = other.data[14];
    data[15] = other.data[15];
    return *this;
}

template<class T>
T *a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::operator[](int index) {
    return &data[index * 4];
}

//
// Const methods
//

template<class T>
a2d::Matrix<4, 4, T> a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::operator*(const Matrix<4, 4, T> &other) const {
    Matrix<4, 4, T> nm;
    nm.data[0] = data[0] * other.data[0] +
                 data[4] * other.data[1] +
                 data[8] * other.data[2] +
                 data[12] * other.data[3];

    nm.data[1] = data[1] * other.data[0] +
                 data[5] * other.data[1] +
                 data[9] * other.data[2] +
                 data[13] * other.data[3];

    nm.data[2] = data[2] * other.data[0] +
                 data[6] * other.data[1] +
                 data[10] * other.data[2] +
                 data[14] * other.data[3];

    nm.data[3] = data[3] * other.data[0] +
                 data[7] * other.data[1] +
                 data[11] * other.data[2] +
                 data[15] * other.data[3];

    nm.data[4] = data[0] * other.data[4] +
                 data[4] * other.data[5] +
                 data[8] * other.data[6] +
                 data[12] * other.data[7];

    nm.data[5] = data[1] * other.data[4] +
                 data[5] * other.data[5] +
                 data[9] * other.data[6] +
                 data[13] * other.data[7];

    nm.data[6] = data[2] * other.data[4] +
                 data[6] * other.data[5] +
                 data[10] * other.data[6] +
                 data[14] * other.data[7];

    nm.data[7] = data[3] * other.data[4] +
                 data[7] * other.data[5] +
                 data[11] * other.data[6] +
                 data[15] * other.data[7];

    nm.data[8] = data[0] * other.data[8] +
                 data[4] * other.data[9] +
                 data[8] * other.data[10] +
                 data[12] * other.data[11];

    nm.data[9] = data[1] * other.data[8] +
                 data[5] * other.data[9] +
                 data[9] * other.data[10] +
                 data[13] * other.data[11];

    nm.data[10] = data[2] * other.data[8] +
                  data[6] * other.data[9] +
                  data[10] * other.data[10] +
                  data[14] * other.data[11];

    nm.data[11] = data[3] * other.data[8] +
                  data[7] * other.data[9] +
                  data[11] * other.data[10] +
                  data[15] * other.data[11];

    nm.data[12] = data[0] * other.data[12] +
                  data[4] * other.data[13] +
                  data[8] * other.data[14] +
                  data[12] * other.data[15];

    nm.data[13] = data[1] * other.data[12] +
                  data[5] * other.data[13] +
                  data[9] * other.data[14] +
                  data[13] * other.data[15];

    nm.data[14] = data[2] * other.data[12] +
                  data[6] * other.data[13] +
                  data[10] * other.data[14] +
                  data[14] * other.data[15];

    nm.data[15] = data[3] * other.data[12] +
                  data[7] * other.data[13] +
                  data[11] * other.data[14] +
                  data[15] * other.data[15];

    return nm;
}

template<class T>
a2d::Vector<4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Transform(T x, T y, T z, T w, Vector<4, T> &dest) const {
    T rx = data[0] * x + data[4] * y + data[8] * z + data[12] * w;
    T ry = data[1] * x + data[5] * y + data[9] * z + data[13] * w;
    T rz = data[2] * x + data[6] * y + data[10] * z + data[14] * w;
    T rw = data[3] * x + data[7] * y + data[11] * z + data[15] * w;
    dest.x = rx;
    dest.y = ry;
    dest.z = rz;
    dest.w = rw;
    return dest;
}

template<class T>
a2d::Vector<4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Transform(Vector<4, T> &v) const {
    return Transform(v, v);
}

template<class T>
a2d::Vector<4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Transform(const Vector<4, T> &v, Vector<4, T> &dest) const {
    T rx = data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12] * v.w;
    T ry = data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13] * v.w;
    T rz = data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14] * v.w;
    T rw = data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15] * v.w;
    dest.x = rx;
    dest.y = ry;
    dest.z = rz;
    dest.w = rw;
    return dest;
}

template<class T>
const T *a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::operator[](int index) const {
    return &data[index * 4];
}

template<class T>
a2d::Matrix<4, 4, T> &a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Invert() {
    T a = data[0] * data[5] - data[1] * data[4];
    T b = data[0] * data[6] - data[2] * data[4];
    T c = data[0] * data[7] - data[3] * data[4];
    T d = data[1] * data[6] - data[2] * data[5];
    T e = data[1] * data[7] - data[3] * data[5];
    T f = data[2] * data[7] - data[3] * data[6];
    T g = data[8] * data[13] - data[9] * data[12];
    T h = data[8] * data[14] - data[10] * data[12];
    T i = data[8] * data[15] - data[11] * data[12];
    T j = data[9] * data[14] - data[10] * data[13];
    T k = data[9] * data[15] - data[11] * data[13];
    T l = data[10] * data[15] - data[11] * data[14];
    T det = a * l - b * k + c * j + d * i - e * h + f * g;
    T nm00, nm01, nm02, nm03, nm10, nm11, nm12, nm13, nm20, nm21, nm22, nm23, nm30, nm31, nm32, nm33;
    det = 1.0f / det;
    nm00 = ( data[5]  * l - data[6]  * k + data[7]  * j) * det;
    nm01 = (-data[1]  * l + data[2]  * k - data[3]  * j) * det;
    nm02 = ( data[13] * f - data[14] * e + data[15] * d) * det;
    nm03 = (-data[9]  * f + data[10] * e - data[11] * d) * det;
    nm10 = (-data[4]  * l + data[6]  * i - data[7]  * h) * det;
    nm11 = ( data[0]  * l - data[2]  * i + data[3]  * h) * det;
    nm12 = (-data[12] * f + data[14] * c - data[15] * b) * det;
    nm13 = ( data[8]  * f - data[10] * c + data[11] * b) * det;
    nm20 = ( data[4]  * k - data[5]  * i + data[7]  * g) * det;
    nm21 = (-data[0]  * k + data[1]  * i - data[3]  * g) * det;
    nm22 = ( data[12] * e - data[13] * c + data[15] * a) * det;
    nm23 = (-data[8]  * e + data[9]  * c - data[11] * a) * det;
    nm30 = (-data[4]  * j + data[5]  * h - data[6]  * g) * det;
    nm31 = ( data[0]  * j - data[1]  * h + data[2]  * g) * det;
    nm32 = (-data[12] * d + data[13] * b - data[14] * a) * det;
    nm33 = ( data[8]  * d - data[9]  * b + data[10] * a) * det;
    data[0] = (nm00);
    data[1] = (nm01);
    data[2] = (nm02);
    data[3] = (nm03);
    data[4] = (nm10);
    data[5] = (nm11);
    data[6] = (nm12);
    data[7] = (nm13);
    data[8] = (nm20);
    data[9] = (nm21);
    data[10] = (nm22);
    data[11] = (nm23);
    data[12] = (nm30);
    data[13] = (nm31);
    data[14] = (nm32);
    data[15] = (nm33);
    return *this;
}

template<class T>
a2d::Vector<4, T> a2d::Matrix<4, 4, T, IF_FLOATING_POINT>::Transform(T x, T y, T z, T w) const {
    Vector<4, T> v;
    return Transform(x, y, z, w, v);
}

template class a2d::Matrix<4, 4, float>;
template class a2d::Matrix<4, 4, double>;
template class a2d::Matrix<4, 4, long double>;