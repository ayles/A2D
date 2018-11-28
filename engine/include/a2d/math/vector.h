//
// Created by selya on 26.10.2018.
//

#ifndef A2D_VECTOR_H
#define A2D_VECTOR_H

#include <type_traits>
#include <cmath>

namespace a2d {

// Vector base template
template<int L, class T>
class Vector;

typedef Vector<2, float> Vector2f;
typedef Vector<3, float> Vector3f;
typedef Vector<4, float> Vector4f;
typedef Vector<2, double> Vector2d;
typedef Vector<3, double> Vector3d;
typedef Vector<4, double> Vector4d;
typedef Vector<2, int> Vector2i;
typedef Vector<3, int> Vector3i;
typedef Vector<4, int> Vector4i;

#define V_ENABLE_IF_FLOATING_POINT \
template<class U = T, class = typename std::enable_if<std::is_floating_point<U>::value>::type>

#define V_INLINE inline

#define V_SQRT(t) std::sqrt(t)
#define V_ROUND(t) std::round(t)
#define V_CEIL(t) std::ceil(t)
#define V_FLOOR(t) std::floor(t)
#define V_ATAN2(y, x) std::atan2(y, x)
#define V_ACOS(t) std::acos(t)

template<class T>
class Vector<2, T> {
public:
    T x, y;

    Vector() : x(0), y(0) {}

    Vector(T scalar) : x(scalar), y(scalar) {}

    Vector(T x, T y) : x(x), y(y) {}

    template<class U>
    Vector(const Vector<2, U> &other) : x(other.x), y(other.y) {}

    //
    // Const member functions
    //

    V_INLINE
    T Length() const {
        return V_SQRT(x * x + y * y);
    }

    V_INLINE
    T LengthSquared() const {
        return x * x + y * y;
    }

    V_INLINE
    T Distance(T x, T y) const {
        T x_diff = this->x - x;
        T y_diff = this->y - y;
        return V_SQRT(x_diff * x_diff + y_diff * y_diff);
    }

    V_INLINE
    T Distance(const Vector<2, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        return V_SQRT(x_diff * x_diff + y_diff * y_diff);
    }

    V_INLINE
    T DistanceSquared(T x, T y) const {
        T x_diff = this->x - x;
        T y_diff = this->y - y;
        return x_diff * x_diff + y_diff * y_diff;
    }

    V_INLINE
    T DistanceSquared(const Vector<2, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        return x_diff * x_diff + y_diff * y_diff;
    }

    V_INLINE
    T Dot(T x, T y) const {
        return this->x * x + this->y * y;
    }

    V_INLINE
    T Dot(const Vector<2, T> &other) const {
        return x * other.x + y * other.y;
    }

    V_INLINE
    T Determinant(T x, T y) const {
        return this->x * y - this->y * x;
    }

    V_INLINE
    T Determinant(const Vector<2, T> &other) const {
        return x * other.y - y * other.x;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Angle(T x, T y) const {
        T dot = this->x * x + this->y * y;
        T det = this->x * y - this->y * x;
        return V_ATAN2(det, dot);
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Angle(const Vector<2, T> &other) const {
        T dot = x * other.x + y * other.y;
        T det = x * other.y - y * other.x;
        return V_ATAN2(det, dot);
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<2, T> Normalized() const {
        T length = V_SQRT(x * x + y * y);
        return Vector<2, T>(x / length, y / length);
    }

    V_INLINE
    Vector<2, T> Perpendicular() const {
        return Vector<2, T>(y, -x);
    }

    V_INLINE
    Vector<2, T> Min(T x, T y) const {
        return Vector<2, T>(this->x > x ? x : this->x, this->y > y ? y : this->y);
    }

    V_INLINE
    Vector<2, T> Min(const Vector<2, T> &other) const {
        return Vector<2, T>(x > other.x ? other.x : x, y > other.y ? other.y : y);
    }

    V_INLINE
    Vector<2, T> Max(T x, T y) const {
        return Vector<2, T>(this->x > x ? this->x : x, this->y > y ? this->y : y);
    }

    V_INLINE
    Vector<2, T> Max(const Vector<2, T> &other) const {
        return Vector<2, T>(x > other.x ? x : other.x, y > other.y ? y : other.y);
    }

    V_INLINE
    Vector<2, T> Lerp(T x, T y, double t) const {
        return Vector<2, T>(this->x + (x - this->x) * t, this->y + (y - this->y) * t);
    }

    V_INLINE
    Vector<2, T> Lerp(const Vector<2, T> &other, double t) const {
        return Vector<2, T>(x + (other.x - x) * t, y + (other.y - y) * t);
    }

    V_INLINE
    Vector<2, T> operator+(const Vector<2, T> &other) const {
        return Vector<2, T>(x + other.x, y + other.y);
    }

    V_INLINE
    Vector<2, T> operator-(const Vector<2, T> &other) const {
        return Vector<2, T>(x - other.x, y - other.y);
    }

    V_INLINE
    Vector<2, T> operator*(const Vector<2, T> &other) const {
        return Vector<2, T>(x * other.x, y * other.y);
    }

    V_INLINE
    Vector<2, T> operator/(const Vector<2, T> &other) const {
        return Vector<2, T>(x / other.x, y / other.y);
    }

    V_INLINE
    Vector<2, T> operator+(T scalar) const {
        return Vector<2, T>(x + scalar, y + scalar);
    }

    V_INLINE
    Vector<2, T> operator-(T scalar) const {
        return Vector<2, T>(x - scalar, y - scalar);
    }

    V_INLINE
    Vector<2, T> operator*(T scalar) const {
        return Vector<2, T>(x * scalar, y * scalar);
    }

    V_INLINE
    Vector<2, T> operator/(T scalar) const {
        return Vector<2, T>(x / scalar, y / scalar);
    }

    V_INLINE
    bool operator==(const Vector<2, T> &other) const {
        return (x == other.x && y == other.y);
    }

    V_INLINE
    bool operator==(T scalar) const {
        return (x == scalar && y == scalar);
    }

    //
    // Non-const member functions
    //


    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<2, T> &Normalize() {
        T length = V_SQRT(x * x + y * y);
        x /= length;
        y /= length;
        return *this;
    }

    V_INLINE
    Vector<2, T> &Zero() {
        x = 0;
        y = 0;
        return *this;
    }

    V_INLINE
    Vector<2, T> &Negate() {
        x = -x;
        y = -y;
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<2, T> &Floor() {
        x = V_FLOOR(x);
        y = V_FLOOR(y);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<2, T> &Ceil() {
        x = V_CEIL(x);
        y = V_CEIL(y);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<2, T> &Round() {
        x = V_ROUND(x);
        y = V_ROUND(y);
        return *this;
    }


    V_INLINE
    Vector<2, T> &Set(T x, T y) {
        this->x = x;
        this->y = y;
        return *this;
    }

    template<class U>
    V_INLINE
    Vector<2, T> &Set(const Vector<2, U> &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &Set(T scalar) {
        x = scalar;
        y = scalar;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator=(const Vector<2, T> &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator+=(const Vector<2, T> &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator-=(const Vector<2, T> &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator*=(const Vector<2, T> &other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator/=(const Vector<2, T> &other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator=(T scalar) {
        x = scalar;
        y = scalar;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator+=(T scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator-=(T scalar) {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    V_INLINE
    Vector<2, T> &operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

// TODO move vector3 and vector4 definition from cpp to h
template<class T>
class Vector<3, T> {
public:
    T x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(T scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector(T x, T y, T z) : x(x), y(y), z(z) {}
    template<class U>
    Vector(const Vector<3, U> &other) : x(other.x), y(other.y), z(other.z) {}
    template<class U>
    Vector(const Vector<2, U> &v2, T z) : x(v2.x), y(v2.y), z(z) {}

    //
    // Const member functions
    //

    V_INLINE
    T Length() const {
        return V_SQRT(x * x + y * y + z * z);
    }

    V_INLINE
    T LengthSquared() const {
        return x * x + y * y + z * z;
    }

    V_INLINE
    T Distance(const Vector<3, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        T z_diff = z - other.z;
        return V_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
    }

    V_INLINE
    T Distance(T x, T y, T z) const {
        T x_diff = this->x - x;
        T y_diff = this->y - y;
        T z_diff = this->z - z;
        return V_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
    }

    V_INLINE
    T DistanceSquared(const Vector<3, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        T z_diff = z - other.z;
        return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
    }

    V_INLINE
    T Dot(const Vector<3, T> &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T AngleCos(const Vector<3, T> &other) const {
        return (x * other.x + y * other.y + z * other.z) /
               V_SQRT((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z));
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Angle(const Vector<3, T> &other) const {
        double cos = (x * other.x + y * other.y + z * other.z) /
                     V_SQRT((x * x + y * y + z * z) * (other.x * other.x + other.y * other.y + other.z * other.z));
        // This is because sometimes cos goes above 1 or below -1 because of lost precision
        cos = cos < 1 ? cos : 1;
        cos = cos > -1 ? cos : -1;
        return (T) V_ACOS(cos);
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<3, T> Normalized() const {
        T length = V_SQRT(x * x + y * y + z * z);
        return Vector<3, T>(x / length, y / length, z / length);
    }

    V_INLINE
    Vector<3, T> Cross(const Vector<3, T> &other) const {
        return Vector<3, T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    V_INLINE
    Vector<3, T> Min(const Vector<3, T> &other) const {
        return Vector<3, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z);
    }

    V_INLINE
    Vector<3, T> Max(const Vector<3, T> &other) const {
        return Vector<3, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z);
    }

    V_INLINE
    Vector<3, T> Lerp(const Vector<3, T> &other, double t) const {
        return Vector<3, T>(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t);
    }


    V_INLINE
    Vector<3, T> operator+(const Vector<3, T> &other) const {
        return Vector<3, T>(x + other.x, y + other.y, z + other.z);
    }

    V_INLINE
    Vector<3, T> operator-(const Vector<3, T> &other) const {
        return Vector<3, T>(x - other.x, y - other.y, z - other.z);
    }

    V_INLINE
    Vector<3, T> operator*(const Vector<3, T> &other) const {
        return Vector<3, T>(x * other.x, y * other.y, z * other.z);
    }

    V_INLINE
    Vector<3, T> operator/(const Vector<3, T> &other) const {
        return Vector<3, T>(x / other.x, y / other.y, z / other.z);
    }


    V_INLINE
    Vector<3, T> operator+(T scalar) const {
        return Vector<3, T>(x + scalar, y + scalar, z + scalar);
    }

    V_INLINE
    Vector<3, T> operator-(T scalar) const {
        return Vector<3, T>(x - scalar, y - scalar, z - scalar);
    }

    V_INLINE
    Vector<3, T> operator*(T scalar) const {
        return Vector<3, T>(x * scalar, y * scalar, z * scalar);
    }

    V_INLINE
    Vector<3, T> operator/(T scalar) const {
        return Vector<3, T>(x / scalar, y / scalar, z / scalar);
    }


    V_INLINE
    bool operator==(const Vector<3, T> &other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    V_INLINE
    bool operator==(T scalar) const {
        return (x == scalar && y == scalar && z == scalar);
    }

//
// Non-const member functions
//

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<3, T> &Normalize() {
        T length = V_SQRT(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

    V_INLINE
    Vector<3, T> &Zero() {
        x = 0;
        y = 0;
        z = 0;
        return *this;
    }

    V_INLINE
    Vector<3, T> &Negate() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<3, T> &Floor() {
        x = V_FLOOR(x);
        y = V_FLOOR(y);
        z = V_FLOOR(z);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<3, T> &Ceil() {
        x = V_CEIL(x);
        y = V_CEIL(y);
        z = V_CEIL(z);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<3, T> &Round() {
        x = V_ROUND(x);
        y = V_ROUND(y);
        z = V_ROUND(z);
        return *this;
    }


    V_INLINE
    Vector<3, T> &Set(T x, T y, T z) {
        this->x = x;
        this->y = y;
        this->z = z;
        return *this;
    }

    V_INLINE
    Vector<3, T> &Set(const a2d::Vector<2, T> &other, T z) {
        this->x = other.x;
        this->y = other.y;
        this->z = z;
        return *this;
    }

    V_INLINE
    Vector<3, T> &Set(T scalar) {
        x = scalar;
        y = scalar;
        z = scalar;
        return *this;
    }


    V_INLINE
    Vector<3, T> &operator=(const Vector<3, T> &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }


    V_INLINE
    Vector<3, T> &operator+=(const Vector<3, T> &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator-=(const Vector<3, T> &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator*=(const Vector<3, T> &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator/=(const Vector<3, T> &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }


    V_INLINE
    Vector<3, T> &operator=(T scalar) {
        x = scalar;
        y = scalar;
        z = scalar;
        return *this;
    }


    V_INLINE
    Vector<3, T> &operator+=(T scalar) {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator-=(T scalar) {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    V_INLINE
    Vector<3, T> &operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};

template<class T>
class Vector<4, T> {
public:
    T x, y, z, w;

    Vector() : x(0), y(0), z(0), w(0) {}
    Vector(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    template<class U>
    Vector(const Vector<4, U> &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
    template<class U>
    Vector(const Vector<3, U> &v3, T w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}
    template<class U>
    Vector(const Vector<2, U> &v2, T z, T w) : x(v2.x), y(v2.y), z(z), w(w) {}

//
// Const member functions
//

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Length() const {
        return V_SQRT(x * x + y * y + z * z + w * w);
    }

    V_INLINE
    T LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Distance(const Vector<4, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        T z_diff = z - other.z;
        T w_diff = w - other.w;
        return V_SQRT(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff);
    }

    V_INLINE
    T DistanceSquared(const Vector<4, T> &other) const {
        T x_diff = x - other.x;
        T y_diff = y - other.y;
        T z_diff = z - other.z;
        T w_diff = w - other.w;
        return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff + w_diff * w_diff;
    }

    V_INLINE
    T Dot(const Vector<4, T> &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T AngleCos(const Vector<4, T> &other) const {
        return (x * other.x + y * other.y + z * other.z + w * other.w) /
               V_SQRT((x * x + y * y + z * z + w * w) * (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w));
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    T Angle(const Vector<4, T> &other) const {
        double cos = (x * other.x + y * other.y + z * other.z + w * other.w) /
                     V_SQRT((x * x + y * y + z * z + w * w) * (other.x * other.x + other.y * other.y + other.z * other.z + other.w * other.w));
        // This is because sometimes cos goes above 1 or below -1 because of lost precision
        cos = cos < 1 ? cos : 1;
        cos = cos > -1 ? cos : -1;
        return (T) acos(cos);
    }


    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<4, T> Normalized() const {
        T length = V_SQRT(x * x + y * y + z * z + w * w);
        return Vector<4, T>(x / length, y / length, z / length, w / length);
    }

    V_INLINE
    Vector<4, T> Min(const Vector<4, T> &other) const {
        return Vector<4, T>(x > other.x ? other.x : x, y > other.y ? other.y : y, z > other.z ? other.z : z, w > other.w ? other.w : w);
    }

    V_INLINE
    Vector<4, T> Max(const Vector<4, T> &other) const {
        return Vector<4, T>(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z, w > other.w ? w : other.w);
    }

    V_INLINE
    Vector<4, T> Lerp(const Vector<4, T> &other, double t) const {
        return Vector<4, T>(x + (other.x - x) * t, y + (other.y - y) * t, z + (other.z - z) * t, w + (other.w - w) * t);
    }


    V_INLINE
    Vector<4, T> operator+(const Vector<4, T> &other) const {
        return Vector<4, T>(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    V_INLINE
    Vector<4, T> operator-(const Vector<4, T> &other) const {
        return Vector<4, T>(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    V_INLINE
    Vector<4, T> operator*(const Vector<4, T> &other) const {
        return Vector<4, T>(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    V_INLINE
    Vector<4, T> operator/(const Vector<4, T> &other) const {
        return Vector<4, T>(x / other.x, y / other.y, z / other.z, w / other.w);
    }


    V_INLINE
    Vector<4, T> operator+(T scalar) const {
        return Vector<4, T>(x + scalar, y + scalar, z + scalar, w + scalar);
    }

    V_INLINE
    Vector<4, T> operator-(T scalar) const {
        return Vector<4, T>(x - scalar, y - scalar, z - scalar, w - scalar);
    }

    V_INLINE
    Vector<4, T> operator*(T scalar) const {
        return Vector<4, T>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    V_INLINE
    Vector<4, T> operator/(T scalar) const {
        return Vector<4, T>(x / scalar, y / scalar, z / scalar, w / scalar);
    }


    V_INLINE
    bool operator==(const Vector<4, T> &other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }

    V_INLINE
    bool operator==(T scalar) const {
        return (x == scalar && y == scalar && z == scalar && w == scalar);
    }

//
// Non-const member functions
//

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<4, T> &Normalize() {
        T length = V_SQRT(x * x + y * y + z * z + w * w);
        x /= length;
        y /= length;
        z /= length;
        w /= length;
        return *this;
    }

    V_INLINE
    Vector<4, T> &Zero() {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
        return *this;
    }

    V_INLINE
    Vector<4, T> &Negate() {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<4, T> &Floor() {
        x = V_FLOOR(x);
        y = V_FLOOR(y);
        z = V_FLOOR(z);
        w = V_FLOOR(w);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<4, T> &Ceil() {
        x = V_CEIL(x);
        y = V_CEIL(y);
        z = V_CEIL(z);
        w = V_CEIL(w);
        return *this;
    }

    V_ENABLE_IF_FLOATING_POINT V_INLINE
    Vector<4, T> &Round() {
        x = V_ROUND(x);
        y = V_ROUND(y);
        z = V_ROUND(z);
        w = V_ROUND(w);
        return *this;
    }


    V_INLINE
    Vector<4, T> &Set(T x, T y, T z, T w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &Set(const a2d::Vector<3, T> &v3, T w) {
        this->x = v3.x;
        this->y = v3.y;
        this->z = v3.z;
        this->w = w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &Set(const a2d::Vector<2, T> &v2, T z, T w) {
        this->x = v2.x;
        this->y = v2.y;
        this->z = z;
        this->w = w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &Set(T scalar) {
        x = scalar;
        y = scalar;
        z = scalar;
        w = scalar;
        return *this;
    }


    V_INLINE
    Vector<4, T> &operator=(const Vector<4, T> &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }


    V_INLINE
    Vector<4, T> &operator+=(const Vector<4, T> &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator-=(const Vector<4, T> &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator*=(const Vector<4, T> &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator/=(const Vector<4, T> &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }


    V_INLINE
    Vector<4, T> &operator=(T scalar) {
        x = scalar;
        y = scalar;
        z = scalar;
        w = scalar;
        return *this;
    }


    V_INLINE
    Vector<4, T> &operator+=(T scalar) {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator-=(T scalar) {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    V_INLINE
    Vector<4, T> &operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
};

} //namespace a2d

#endif //A2D_VECTOR_H
