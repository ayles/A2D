//
// Created by selya on 26.10.2018.
//

#include <A2D/math/vector.h>

#include <cmath>

#define IF_FLOATING_POINT typename std::enable_if<std::is_floating_point<T>::value>::type
#define IF_INTEGRAL typename std::enable_if<std::is_integral<T>::value>::type

// Vector template specialization for Vec3f

template<class T>
a2d::Vector<3, T, IF_FLOATING_POINT>::Vector() {}
template<class T>
a2d::Vector<3, T, IF_FLOATING_POINT>::Vector(T scalar) : x(scalar), y(scalar), z(scalar) {}
template<class T>
a2d::Vector<3, T, IF_FLOATING_POINT>::Vector(T x, T y, T z) : x(x), y(y), z(z) {}
template<class T>
a2d::Vector<3, T, IF_FLOATING_POINT>::Vector(const Vector<3, T> &other) : x(other.x), y(other.y), z(other.z) {}
template<class T>
a2d::Vector<3, T, IF_FLOATING_POINT>::Vector(const Vector<2, T> &v2, T z) : x(v2.x), y(v2.y), z(z) {}

//
// Const member functions
//

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::Length() const {
    return std::sqrt(x * x + y * y + z * z);
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::LengthSquared() const {
    return x * x + y * y + z * z;
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::Distance(const Vector<3, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::DistanceSquared(const Vector<3, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::Dot(const Vector<3, T> &other) const {
    return x * other.x + y * other.y + z * other.z;
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::AngleCos(const Vector<3, T> &other) const {
    return (x * other.x + y * other.y + z * other.z) /
           std::sqrt((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z));
}

template<class T>
T a2d::Vector<3, T, IF_FLOATING_POINT>::Angle(const Vector<3, T> &other) const {
    double cos = (x * other.x + y * other.y + z * other.z) /
                 std::sqrt((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z));
    // This is because sometimes cos goes above 1 or below -1 because of lost precision
    cos = cos < 1 ? cos : 1;
    cos = cos > -1 ? cos : -1;
    return (T) acos(cos);
}


template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::Normalized() const {
    T length = std::sqrt(x * x + y * y + z * z);
    return Vector<3, T>(x / length, y / length, z / length);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::Cross(const Vector<3, T> &other) const {
    return Vector<3, T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::Min(const Vector<3, T> &other) const {
    return Vector<3, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::Max(const Vector<3, T> &other) const {
    return Vector<3, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::Lerp(const Vector<3, T> &other, T t) const {
    return Vector<3, T>(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t);
}


template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator+(const Vector<3, T> &other) const {
    return Vector<3, T>(x + other.x, y + other.y, z + other.z);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator-(const Vector<3, T> &other) const {
    return Vector<3, T>(x - other.x, y - other.y, z - other.z);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator*(const Vector<3, T> &other) const {
    return Vector<3, T>(x * other.x, y * other.y, z * other.z);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator/(const Vector<3, T> &other) const {
    return Vector<3, T>(x / other.x, y / other.y, z / other.z);
}


template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator+(T scalar) const {
    return Vector<3, T>(x + scalar, y + scalar, z + scalar);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator-(T scalar) const {
    return Vector<3, T>(x - scalar, y - scalar, z - scalar);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator*(T scalar) const {
    return Vector<3, T>(x * scalar, y * scalar, z * scalar);
}

template<class T>
a2d::Vector<3, T> a2d::Vector<3, T, IF_FLOATING_POINT>::operator/(T scalar) const {
    return Vector<3, T>(x / scalar, y / scalar, z / scalar);
}


template<class T>
bool a2d::Vector<3, T, IF_FLOATING_POINT>::operator==(const Vector<3, T> &other) const {
    return (x == other.x && y == other.y && z == other.z);
}

template<class T>
bool a2d::Vector<3, T, IF_FLOATING_POINT>::operator==(T scalar) const {
    return (x == scalar && y == scalar && z == scalar);
}

//
// Non-const member functions
//

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Normalize() {
    T length = std::sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Zero() {
    x = 0;
    y = 0;
    z = 0;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Negate() {
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Floor() {
    x = std::floor(x);
    y = std::floor(y);
    z = std::floor(z);
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Ceil() {
    x = std::ceil(x);
    y = std::ceil(y);
    z = std::ceil(z);
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Round() {
    x = std::round(x);
    y = std::round(y);
    z = std::round(z);
    return *this;
}


template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Set(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Set(const a2d::Vector<2, T> &other, T z) {
    this->x = other.x;
    this->y = other.y;
    this->z = z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::Set(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    return *this;
}


template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator=(const Vector<3, T> &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}


template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator+=(const Vector<3, T> &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator-=(const Vector<3, T> &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator*=(const Vector<3, T> &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator/=(const Vector<3, T> &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}


template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    return *this;
}


template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

template<class T>
a2d::Vector<3, T> &a2d::Vector<3, T, IF_FLOATING_POINT>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

template class a2d::Vector<3, float>;
template class a2d::Vector<3, double>;
template class a2d::Vector<3, long double>;
template class a2d::Vector<3, signed char>;
template class a2d::Vector<3, unsigned char>;
template class a2d::Vector<3, signed short>;
template class a2d::Vector<3, unsigned short>;
template class a2d::Vector<3, signed int>;
template class a2d::Vector<3, unsigned int>;
template class a2d::Vector<3, signed long int>;
template class a2d::Vector<3, unsigned long int>;
template class a2d::Vector<3, signed long long int>;
template class a2d::Vector<3, unsigned long long int>;