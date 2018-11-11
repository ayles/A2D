//
// Created by selya on 26.10.2018.
//

#include <A2D/math/vector.h>

#include <cmath>

#define IF_FLOATING_POINT typename std::enable_if<std::is_floating_point<T>::value>::type
#define IF_INTEGRAL typename std::enable_if<std::is_integral<T>::value>::type

// Vector template specialization for Vec3f

template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector() {}
template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector(const Vector<4, T> &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector(const a2d::Vector<3, T> &v3, T w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}
template<class T>
a2d::Vector<4, T, IF_FLOATING_POINT>::Vector(const Vector<2, T> &v2, T z, T w) : x(v2.x), y(v2.y), z(z), w(w) {}

//
// Const member functions
//

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::Length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::LengthSquared() const {
    return x * x + y * y + z * z + w * w;
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::Distance(const Vector<4, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    T w_diff = w - other.w;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff);
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::DistanceSquared(const Vector<4, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    T w_diff = w - other.w;
    return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff;
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::Dot(const Vector<4, T> &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::AngleCos(const Vector<4, T> &other) const {
    return (x * other.x + y * other.y + z * other.z + w * other.w) /
           std::sqrt((x * x + y * y + z * z + w * w) * (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w));
}

template<class T>
T a2d::Vector<4, T, IF_FLOATING_POINT>::Angle(const Vector<4, T> &other) const {
    double cos = (x * other.x + y * other.y + z * other.z + w * other.w) /
                 std::sqrt((x * x + y * y + z * z + w * w) * (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w));
    // This is because sometimes cos goes above 1 or below -1 because of lost precision
    cos = cos < 1 ? cos : 1;
    cos = cos > -1 ? cos : -1;
    return (T) acos(cos);
}


template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::Normalized() const {
    T length = std::sqrt(x * x + y * y + z * z + w * w);
    return Vector<4, T>(x / length, y / length, z / length, w / length);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::Min(const Vector<4, T> &other) const {
    return Vector<4, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z, w > other.w ? other.w : w);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::Max(const Vector<4, T> &other) const {
    return Vector<4, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z, w > other.w ? w : other.w);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::Lerp(const Vector<4, T> &other, T t) const {
    return Vector<4, T>(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t, w + (other.w - w) * t);
}


template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator+(const Vector<4, T> &other) const {
    return Vector<4, T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator-(const Vector<4, T> &other) const {
    return Vector<4, T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator*(const Vector<4, T> &other) const {
    return Vector<4, T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator/(const Vector<4, T> &other) const {
    return Vector<4, T>(x / other.x, y / other.y, z / other.z, w / other.w);
}


template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator+(T scalar) const {
    return Vector<4, T>(x + scalar, y + scalar, z + scalar, w + scalar);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator-(T scalar) const {
    return Vector<4, T>(x - scalar, y - scalar, z - scalar, w - scalar);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator*(T scalar) const {
    return Vector<4, T>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template<class T>
a2d::Vector<4, T> a2d::Vector<4, T, IF_FLOATING_POINT>::operator/(T scalar) const {
    return Vector<4, T>(x / scalar, y / scalar, z / scalar, w / scalar);
}


template<class T>
bool a2d::Vector<4, T, IF_FLOATING_POINT>::operator==(const Vector<4, T> &other) const {
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

template<class T>
bool a2d::Vector<4, T, IF_FLOATING_POINT>::operator==(T scalar) const {
    return (x == scalar && y == scalar && z == scalar && w == scalar);
}

//
// Non-const member functions
//

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Normalize() {
    T length = std::sqrt(x * x + y * y + z * z + w * w);
    x /= length;
    y /= length;
    z /= length;
    w /= length;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Zero() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Negate() {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Floor() {
    x = std::floor(x);
    y = std::floor(y);
    z = std::floor(z);
    w = std::floor(w);
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Ceil() {
    x = std::ceil(x);
    y = std::ceil(y);
    z = std::ceil(z);
    w = std::ceil(w);
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Round() {
    x = std::round(x);
    y = std::round(y);
    z = std::round(z);
    w = std::round(w);
    return *this;
}


template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Set(T x, T y, T z, T w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Set(const a2d::Vector<3, T> &v3, T w) {
    this->x = v3.x;
    this->y = v3.y;
    this->z = v3.z;
    this->w = w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Set(const a2d::Vector<2, T> &v2, T z, T w) {
    this->x = v2.x;
    this->y = v2.y;
    this->z = z;
    this->w = w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::Set(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    w = scalar;
    return *this;
}


template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator=(const Vector<4, T> &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}


template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator+=(const Vector<4, T> &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator-=(const Vector<4, T> &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator*=(const Vector<4, T> &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator/=(const Vector<4, T> &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
}


template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    w = scalar;
    return *this;
}


template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

template<class T>
a2d::Vector<4, T> &a2d::Vector<4, T, IF_FLOATING_POINT>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

template class a2d::Vector<4, float>;
template class a2d::Vector<4, double>;
template class a2d::Vector<4, long double>;
template class a2d::Vector<4, signed char>;
template class a2d::Vector<4, unsigned char>;
template class a2d::Vector<4, signed short>;
template class a2d::Vector<4, unsigned short>;
template class a2d::Vector<4, signed int>;
template class a2d::Vector<4, unsigned int>;
template class a2d::Vector<4, signed long int>;
template class a2d::Vector<4, unsigned long int>;
template class a2d::Vector<4, signed long long int>;
template class a2d::Vector<4, unsigned long long int>;