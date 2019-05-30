//
// Created by selya on 23.05.2019.
//

#include <a2d/math/vector.hpp>

namespace a2d {




// ----------------------------------------------- Vector<2, T> ------------------------------------------------




template<class T>
Vector<2, T>::Vector() : x(0), y(0) {}

template<class T>
Vector<2, T>::Vector(T x, T y) : x(x), y(y) {}

template<class T>
Vector<2, T>::Vector(T scalar) : x(scalar), y(scalar) {}

template<class T>
Vector<2, T>::Vector(const Vector<2, T> &other) : x((T)other.x), y((T)other.y) {}

template<class T>
T Vector<2, T>::Length() const {
    return (T)A2D_MATH_SQRT(x * x + y * y);
}

template<class T>
T Vector<2, T>::LengthSquared() const {
    return x * x + y * y;
}

template<class T>
T Vector<2, T>::Distance(T x, T y) const {
    T x_diff = this->x - x;
    T y_diff = this->y - y;
    return (T)A2D_MATH_SQRT(x_diff * x_diff + y_diff * y_diff);
}

template<class T>
T Vector<2, T>::Distance(const Vector<2, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    return (T)A2D_MATH_SQRT(x_diff * x_diff + y_diff * y_diff);
}

template<class T>
T Vector<2, T>::DistanceSquared(T x, T y) const {
    T x_diff = this->x - x;
    T y_diff = this->y - y;
    return x_diff * x_diff + y_diff * y_diff;
}

template<class T>
T Vector<2, T>::DistanceSquared(const Vector<2, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    return x_diff * x_diff + y_diff * y_diff;
}

template<class T>
T Vector<2, T>::Dot(T x, T y) const {
    return this->x * x + this->y * y;
}

template<class T>
T Vector<2, T>::Dot(const Vector<2, T> &other) const {
    return x * other.x + y * other.y;
}

template<class T>
T Vector<2, T>::Determinant(T x, T y) const {
    return this->x * y - this->y * x;
}

template<class T>
T Vector<2, T>::Determinant(const Vector<2, T> &other) const {
    return x * other.y - y * other.x;
}

template<class T>
T Vector<2, T>::Angle(T x, T y) const {
    T dot = this->x * x + this->y * y;
    T det = this->x * y - this->y * x;
    return (T)A2D_MATH_ATAN2(det, dot);
}

template<class T>
T Vector<2, T>::Angle(const Vector<2, T> &other) const {
    T dot = x * other.x + y * other.y;
    T det = x * other.y - y * other.x;
    return (T)A2D_MATH_ATAN2(det, dot);
}

template<class T>
Vector<2, T> Vector<2, T>::Normalized() const {
    T length = (T)A2D_MATH_SQRT(x * x + y * y);
    return Vector<2, T>(x / length, y / length);
}

template<class T>
Vector<2, T> Vector<2, T>::Perpendicular() const {
    return Vector<2, T>(y, -x);
}

template<class T>
Vector<2, T> Vector<2, T>::Min(T x, T y) const {
    return Vector<2, T>(this->x > x ? x : this->x, this->y > y ? y : this->y);
}

template<class T>
Vector<2, T> Vector<2, T>::Min(const Vector<2, T> &other) const {
    return Vector<2, T>(x > other.x ? other.x : x, y > other.y ? other.y : y);
}

template<class T>
Vector<2, T> Vector<2, T>::Max(T x, T y) const {
    return Vector<2, T>(this->x > x ? this->x : x, this->y > y ? this->y : y);
}

template<class T>
Vector<2, T> Vector<2, T>::Max(const Vector<2, T> &other) const {
    return Vector<2, T>(x > other.x ? x : other.x, y > other.y ? y : other.y);
}

template<class T>
Vector<2, T> Vector<2, T>::Lerp(T x, T y, double t) const {
    return Vector<2, T>((T)(this->x + (x - this->x) * t), (T)(this->y + (y - this->y) * t));
}

template<class T>
Vector<2, T> Vector<2, T>::Lerp(const Vector<2, T> &other, double t) const {
    return Vector<2, T>((T)(x + (other.x - x) * t), (T)(y + (other.y - y) * t));
}

template<class T>
Vector<2, T> Vector<2, T>::operator+(const Vector<2, T> &other) const {
    return Vector<2, T>(x + other.x, y + other.y);
}

template<class T>
Vector<2, T> Vector<2, T>::operator-(const Vector<2, T> &other) const {
    return Vector<2, T>(x - other.x, y - other.y);
}

template<class T>
Vector<2, T> Vector<2, T>::operator*(const Vector<2, T> &other) const {
    return Vector<2, T>(x * other.x, y * other.y);
}

template<class T>
Vector<2, T> Vector<2, T>::operator/(const Vector<2, T> &other) const {
    return Vector<2, T>(x / other.x, y / other.y);
}

template<class T>
Vector<2, T> Vector<2, T>::operator+(T scalar) const {
    return Vector<2, T>(x + scalar, y + scalar);
}

template<class T>
Vector<2, T> Vector<2, T>::operator-(T scalar) const {
    return Vector<2, T>(x - scalar, y - scalar);
}

template<class T>
Vector<2, T> Vector<2, T>::operator*(T scalar) const {
    return Vector<2, T>(x * scalar, y * scalar);
}

template<class T>
Vector<2, T> Vector<2, T>::operator/(T scalar) const {
    return Vector<2, T>(x / scalar, y / scalar);
}

template<class T>
bool Vector<2, T>::operator==(const Vector<2, T> &other) const {
    return (x == other.x && y == other.y);
}

template<class T>
bool Vector<2, T>::operator==(T scalar) const {
    return (x == scalar && y == scalar);
}

template<class T>
Vector<2, T> &Vector<2, T>::Normalize() {
    T length = (T)A2D_MATH_SQRT(x * x + y * y);
    x /= length;
    y /= length;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Zero() {
    x = 0;
    y = 0;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Floor() {
    x = (T)A2D_MATH_FLOOR(x);
    y = (T)A2D_MATH_FLOOR(y);
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Ceil() {
    x = (T)A2D_MATH_CEIL(x);
    y = (T)A2D_MATH_CEIL(y);
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Round() {
    x = (T)A2D_MATH_ROUND(x);
    y = (T)A2D_MATH_ROUND(y);
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Set(T x, T y) {
    this->x = x;
    this->y = y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Set(const Vector<2, T> &other) {
    x = (T)other.x;
    y = (T)other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::Set(T scalar) {
    x = scalar;
    y = scalar;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator-() {
    x = -x;
    y = -y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator=(const Vector<2, T> &other) {
    x = other.x;
    y = other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator+=(const Vector<2, T> &other) {
    x += other.x;
    y += other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator-=(const Vector<2, T> &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator*=(const Vector<2, T> &other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator/=(const Vector<2, T> &other) {
    x /= other.x;
    y /= other.y;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

template<class T>
Vector<2, T> &Vector<2, T>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}




// ----------------------------------------------- Vector<3, T> ------------------------------------------------




template<class T>
Vector<3, T>::Vector() : x(0), y(0), z(0) {}

template<class T>
Vector<3, T>::Vector(T scalar) : x(scalar), y(scalar), z(scalar) {}

template<class T>
Vector<3, T>::Vector(T x, T y, T z) : x(x), y(y), z(z) {}

template<class T>
Vector<3, T>::Vector(const Vector<3, T> &other) : x((T)other.x), y((T)other.y), z((T)other.z) {}

template<class T>
Vector<3, T>::Vector(const Vector<2, T> &v2, T z) : x((T)v2.x), y((T)v2.y), z(z) {}

template<class T>
T Vector<3, T>::Length() const {
    return (T)A2D_MATH_SQRT(x * x + y * y + z * z);
}

template<class T>
T Vector<3, T>::LengthSquared() const {
    return x * x + y * y + z * z;
}

template<class T>
T Vector<3, T>::Distance(const Vector<3, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    return (T)A2D_MATH_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

template<class T>
T Vector<3, T>::Distance(T x, T y, T z) const {
    T x_diff = this->x - x;
    T y_diff = this->y - y;
    T z_diff = this->z - z;
    return (T)A2D_MATH_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

template<class T>
T Vector<3, T>::DistanceSquared(const Vector<3, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
}

template<class T>
T Vector<3, T>::Dot(const Vector<3, T> &other) const {
    return x * other.x + y * other.y + z * other.z;
}

template<class T>
T Vector<3, T>::AngleCos(const Vector<3, T> &other) const {
    return (T)((x * other.x + y * other.y + z * other.z) /
           A2D_MATH_SQRT((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z)));
}

template<class T>
T Vector<3, T>::Angle(const Vector<3, T> &other) const {
    double cos = (x * other.x + y * other.y + z * other.z) /
                 A2D_MATH_SQRT((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z));
    // This is because sometimes cos goes above 1 or below -1 because of lost precision
    cos = cos < 1 ? cos : 1;
    cos = cos > -1 ? cos : -1;
    return (T)A2D_MATH_ACOS(cos);
}

template<class T>
Vector<3, T> Vector<3, T>::Normalized() const {
    T length = (T)A2D_MATH_SQRT(x * x + y * y + z * z);
    return Vector<3, T>(x / length, y / length, z / length);
}

template<class T>
Vector<3, T> Vector<3, T>::Cross(const Vector<3, T> &other) const {
    return Vector<3, T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

template<class T>
Vector<3, T> Vector<3, T>::Min(const Vector<3, T> &other) const {
    return Vector<3, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z);
}

template<class T>
Vector<3, T> Vector<3, T>::Max(const Vector<3, T> &other) const {
    return Vector<3, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z);
}

template<class T>
Vector<3, T> Vector<3, T>::Lerp(const Vector<3, T> &other, double t) const {
    return Vector<3, T>((T)(x + (other.x - x) * t), (T)(y + (other.y - y) * t), (T)(z + (other.z - z) * t));
}

template<class T>
Vector<3, T> Vector<3, T>::operator+(const Vector<3, T> &other) const {
    return Vector<3, T>(x + other.x, y + other.y, z + other.z);
}

template<class T>
Vector<3, T> Vector<3, T>::operator-(const Vector<3, T> &other) const {
    return Vector<3, T>(x - other.x, y - other.y, z - other.z);
}

template<class T>
Vector<3, T> Vector<3, T>::operator*(const Vector<3, T> &other) const {
    return Vector<3, T>(x * other.x, y * other.y, z * other.z);
}

template<class T>
Vector<3, T> Vector<3, T>::operator/(const Vector<3, T> &other) const {
    return Vector<3, T>(x / other.x, y / other.y, z / other.z);
}

template<class T>
Vector<3, T> Vector<3, T>::operator+(T scalar) const {
    return Vector<3, T>(x + scalar, y + scalar, z + scalar);
}

template<class T>
Vector<3, T> Vector<3, T>::operator-(T scalar) const {
    return Vector<3, T>(x - scalar, y - scalar, z - scalar);
}

template<class T>
Vector<3, T> Vector<3, T>::operator*(T scalar) const {
    return Vector<3, T>(x * scalar, y * scalar, z * scalar);
}

template<class T>
Vector<3, T> Vector<3, T>::operator/(T scalar) const {
    return Vector<3, T>(x / scalar, y / scalar, z / scalar);
}

template<class T>
bool Vector<3, T>::operator==(const Vector<3, T> &other) const {
    return (x == other.x && y == other.y && z == other.z);
}

template<class T>
bool Vector<3, T>::operator==(T scalar) const {
    return (x == scalar && y == scalar && z == scalar);
}

template<class T>
Vector<3, T> &Vector<3, T>::Normalize() {
    T length = (T)A2D_MATH_SQRT(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Zero() {
    x = 0;
    y = 0;
    z = 0;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Floor() {
    x = (T)A2D_MATH_FLOOR(x);
    y = (T)A2D_MATH_FLOOR(y);
    z = (T)A2D_MATH_FLOOR(z);
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Ceil() {
    x = (T)A2D_MATH_CEIL(x);
    y = (T)A2D_MATH_CEIL(y);
    z = (T)A2D_MATH_CEIL(z);
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Round() {
    x = (T)A2D_MATH_ROUND(x);
    y = (T)A2D_MATH_ROUND(y);
    z = (T)A2D_MATH_ROUND(z);
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Set(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Set(const a2d::Vector<3, T> &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Set(const Vector<2, T> &other, T z) {
    this->x = other.x;
    this->y = other.y;
    this->z = z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::Set(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator-() {
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator=(const Vector<3, T> &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator+=(const Vector<3, T> &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator-=(const Vector<3, T> &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator*=(const Vector<3, T> &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator/=(const Vector<3, T> &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

template<class T>
Vector<3, T> &Vector<3, T>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}




// ----------------------------------------------- Vector<4, T> ------------------------------------------------




template<class T>
Vector<4, T>::Vector() : x(0), y(0), z(0), w(0) {}

template<class T>
Vector<4, T>::Vector(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

template<class T>
Vector<4, T>::Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

template<class T>
Vector<4, T>::Vector(const Vector<4, T> &other) : x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w) {}

template<class T>
Vector<4, T>::Vector(const Vector<3, T> &v3, T w) : x((T)v3.x), y((T)v3.y), z((T)v3.z), w(w) {}

template<class T>
Vector<4, T>::Vector(const Vector<2, T> &v2, T z, T w) : x((T)v2.x), y((T)v2.y), z(z), w(w) {}

template<class T>
T Vector<4, T>::Length() const {
    return (T)A2D_MATH_SQRT(x * x + y * y + z * z + w * w);
}

template<class T>
T Vector<4, T>::LengthSquared() const {
    return x * x + y * y + z * z + w * w;
}

template<class T>
T Vector<4, T>::Distance(const Vector<4, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    T w_diff = w - other.w;
    return (T)A2D_MATH_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff);
}

template<class T>
T Vector<4, T>::DistanceSquared(const Vector<4, T> &other) const {
    T x_diff = x - other.x;
    T y_diff = y - other.y;
    T z_diff = z - other.z;
    T w_diff = w - other.w;
    return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff;
}

template<class T>
T Vector<4, T>::Dot(const Vector<4, T> &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

template<class T>
T Vector<4, T>::AngleCos(const Vector<4, T> &other) const {
    return (T)((x * other.x + y * other.y + z * other.z + w * other.w) /
           A2D_MATH_SQRT((x * x + y * y + z * z + w * w) *
                         (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w)));
}

template<class T>
T Vector<4, T>::Angle(const Vector<4, T> &other) const {
    double cos = (x * other.x + y * other.y + z * other.z + w * other.w) /
                 A2D_MATH_SQRT((x * x + y * y + z * z + w * w) *
                               (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w));
    // This is because sometimes cos goes above 1 or below -1 because of lost precision
    cos = cos < 1 ? cos : 1;
    cos = cos > -1 ? cos : -1;
    return (T)A2D_MATH_ACOS(cos);
}

template<class T>
Vector<4, T> Vector<4, T>::Normalized() const {
    T length = (T)A2D_MATH_SQRT(x * x + y * y + z * z + w * w);
    return Vector<4, T>(x / length, y / length, z / length, w / length);
}

template<class T>
Vector<4, T> Vector<4, T>::Min(const Vector<4, T> &other) const {
    return Vector<4, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z,
                        w > other.w ? other.w : w);
}

template<class T>
Vector<4, T> Vector<4, T>::Max(const Vector<4, T> &other) const {
    return Vector<4, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z,
                        w > other.w ? w : other.w);
}

template<class T>
Vector<4, T> Vector<4, T>::Lerp(const Vector<4, T> &other, double t) const {
    return Vector<4, T>((T)(x + (other.x - x) * t), (T)(y + (other.y - y) * t), (T)(z + (other.z - z) * t),
                        (T)(w + (other.w - w) * t));
}

template<class T>
Vector<4, T> Vector<4, T>::operator+(const Vector<4, T> &other) const {
    return Vector<4, T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<class T>
Vector<4, T> Vector<4, T>::operator-(const Vector<4, T> &other) const {
    return Vector<4, T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<class T>
Vector<4, T> Vector<4, T>::operator*(const Vector<4, T> &other) const {
    return Vector<4, T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<class T>
Vector<4, T> Vector<4, T>::operator/(const Vector<4, T> &other) const {
    return Vector<4, T>(x / other.x, y / other.y, z / other.z, w / other.w);
}

template<class T>
Vector<4, T> Vector<4, T>::operator+(T scalar) const {
    return Vector<4, T>(x + scalar, y + scalar, z + scalar, w + scalar);
}

template<class T>
Vector<4, T> Vector<4, T>::operator-(T scalar) const {
    return Vector<4, T>(x - scalar, y - scalar, z - scalar, w - scalar);
}

template<class T>
Vector<4, T> Vector<4, T>::operator*(T scalar) const {
    return Vector<4, T>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template<class T>
Vector<4, T> Vector<4, T>::operator/(T scalar) const {
    return Vector<4, T>(x / scalar, y / scalar, z / scalar, w / scalar);
}

template<class T>
bool Vector<4, T>::operator==(const Vector<4, T> &other) const {
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

template<class T>
bool Vector<4, T>::operator==(T scalar) const {
    return (x == scalar && y == scalar && z == scalar && w == scalar);
}

template<class T>
Vector<4, T> &Vector<4, T>::Normalize() {
    T length = (T)A2D_MATH_SQRT(x * x + y * y + z * z + w * w);
    x /= length;
    y /= length;
    z /= length;
    w /= length;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Zero() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Floor() {
    x = (T)A2D_MATH_FLOOR(x);
    y = (T)A2D_MATH_FLOOR(y);
    z = (T)A2D_MATH_FLOOR(z);
    w = (T)A2D_MATH_FLOOR(w);
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Ceil() {
    x = (T)A2D_MATH_CEIL(x);
    y = (T)A2D_MATH_CEIL(y);
    z = (T)A2D_MATH_CEIL(z);
    w = (T)A2D_MATH_CEIL(w);
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Round() {
    x = (T)A2D_MATH_ROUND(x);
    y = (T)A2D_MATH_ROUND(y);
    z = (T)A2D_MATH_ROUND(z);
    w = (T)A2D_MATH_ROUND(w);
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Set(T x, T y, T z, T w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Set(const Vector<4, T> &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Set(const Vector<3, T> &other, T w) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Set(const Vector<2, T> &other, T z, T w) {
    this->x = other.x;
    this->y = other.y;
    this->z = z;
    this->w = w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::Set(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    w = scalar;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator-() {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator=(const Vector<4, T> &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator+=(const Vector<4, T> &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator-=(const Vector<4, T> &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator*=(const Vector<4, T> &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator/=(const Vector<4, T> &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator=(T scalar) {
    x = scalar;
    y = scalar;
    z = scalar;
    w = scalar;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

template<class T>
Vector<4, T> &Vector<4, T>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

template
class Vector<2, float>;

template
class Vector<3, float>;

template
class Vector<4, float>;

template
class Vector<2, double>;

template
class Vector<3, double>;

template
class Vector<4, double>;

template
class Vector<2, int>;

template
class Vector<3, int>;

template
class Vector<4, int>;

template
class Vector<2, unsigned int>;

template
class Vector<3, unsigned int>;

template
class Vector<4, unsigned int>;

}
