//
// Created by selya on 26.10.2018.
//

#include <a2d/math/vector.h>

#include <cmath>

#define IF_FLOATING_POINT typename std::enable_if<std::is_floating_point<T>::value>::type
#define IF_INTEGRAL typename std::enable_if<std::is_integral<T>::value>::type


// Vector template specialization for Vec2f
template<class T>
a2d::Vector<2, T, IF_FLOATING_POINT>::Vector() : x(0), y(0) {}
template<class T>
a2d::Vector<2, T, IF_FLOATING_POINT>::Vector(T scalar) : x(scalar), y(scalar) {}
template<class T>
a2d::Vector<2, T, IF_FLOATING_POINT>::Vector(T x, T y) : x(x), y(y) {}
template<class T>
a2d::Vector<2, T, IF_FLOATING_POINT>::Vector(const Vector<2, T> &other) : x(other.x), y(other.y) {}

//
// Const member functions
//

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::Length() const {
    return std::sqrt(x * x + y * y);
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::LengthSquared() const {
    return x * x + y * y;
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::Distance(const Vector<2, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::DistanceSquared(const Vector<2, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    return x_diff * x_diff + y_diff * y_diff;
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::Dot(const Vector<2, T> &other) const {
    return x * other.x + y * other.y;
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::Determinant(const a2d::Vector<2, T> &other) const {
    return x * other.y - y * other.x;
}

template<class T>
T a2d::Vector<2, T, IF_FLOATING_POINT>::Angle(const Vector<2, T> &other) const {
    T dot = x * other.x + y * other.y;
    T det = x * other.y - y * other.x;
    return std::atan2(det, dot);
}


template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::Normalized() const {
    T length = std::sqrt(x * x + y * y);
    return Vector<2, T>(x / length, y / length);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::Perpendicular() const {
    return Vector<2, T>(y, -x);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::Min(const Vector<2, T> &other) const {
    return Vector<2, T>(x > other.x ? other.x : x, y > other.y ? other.y : y);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::Max(const Vector<2, T> &other) const {
    return Vector<2, T>(x > other.x ? x : other.x, y > other.y ? y : other.y);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::Lerp(const Vector<2, T> &other, double t) const {
    return Vector<2, T>(x + (other.x - x) * t, y + (other.y - y) * t);
}


template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator+(const Vector<2, T> &other) const {
    return Vector<2, T>(x + other.x, y + other.y);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator-(const Vector<2, T> &other) const {
    return Vector<2, T>(x - other.x, y - other.y);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator*(const Vector<2, T> &other) const {
    return Vector<2, T>(x * other.x, y * other.y);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator/(const Vector<2, T> &other) const {
    return Vector<2, T>(x / other.x, y / other.y);
}


template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator+(T scalar) const {
    return Vector<2, T>(x + scalar, y + scalar);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator-(T scalar) const {
    return Vector<2, T>(x - scalar, y - scalar);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator*(T scalar) const {
    return Vector<2, T>(x * scalar, y * scalar);
}

template<class T>
a2d::Vector<2, T> a2d::Vector<2, T, IF_FLOATING_POINT>::operator/(T scalar) const {
    return Vector<2, T>(x / scalar, y / scalar);
}


template<class T>
bool a2d::Vector<2, T, IF_FLOATING_POINT>::operator==(const a2d::Vector<2, T> &other) const {
    return (x == other.x && y == other.y);
}

template<class T>
bool a2d::Vector<2, T, IF_FLOATING_POINT>::operator==(T scalar) const {
    return (x == scalar && y == scalar);
}

//
// Non-const member functions
//

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Normalize() {
    T length = std::sqrt(x * x + y * y);
    x /= length;
    y /= length;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Zero() {
    x = 0;
    y = 0;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Negate() {
    x = -x;
    y = -y;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Floor() {
    x = std::floor(x);
    y = std::floor(y);
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Ceil() {
    x = std::ceil(x);
    y = std::ceil(y);
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Round() {
    x = std::round(x);
    y = std::round(y);
    return *this;
}


template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Set(T x, T y) {
    this->x = x;
    this->y = y;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::Set(T scalar) {
    x = scalar;
    y = scalar;
    return *this;
}


template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator=(const a2d::Vector<2, T> &other) {
    x = other.x;
    y = other.y;
    return *this;
}


template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator+=(const Vector<2, T> &other) {
    x += other.x;
    y += other.y;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator-=(const Vector<2, T> &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator*=(const Vector<2, T> &other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator/=(const Vector<2, T> &other) {
    x /= other.x;
    y /= other.y;
    return *this;
}


template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    return *this;
}


template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

template<class T>
a2d::Vector<2, T> &a2d::Vector<2, T, IF_FLOATING_POINT>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

template class a2d::Vector<2, float>;
template class a2d::Vector<2, double>;
template class a2d::Vector<2, long double>;
template class a2d::Vector<2, signed char>;
template class a2d::Vector<2, unsigned char>;
template class a2d::Vector<2, signed short>;
template class a2d::Vector<2, unsigned short>;
template class a2d::Vector<2, signed int>;
template class a2d::Vector<2, unsigned int>;
template class a2d::Vector<2, signed long int>;
template class a2d::Vector<2, unsigned long int>;
template class a2d::Vector<2, signed long long int>;
template class a2d::Vector<2, unsigned long long int>;