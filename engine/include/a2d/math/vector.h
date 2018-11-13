//
// Created by selya on 26.10.2018.
//

#ifndef A2D_VECTOR_H
#define A2D_VECTOR_H

#include <type_traits>

namespace a2d {

// Vector base template
template<int L, class T, class F = void>
class Vector;

typedef Vector<2, float> Vector2f;
typedef Vector<3, float> Vector3f;
typedef Vector<4, float> Vector4f;
typedef Vector<2, double> Vector2d;
typedef Vector<3, double> Vector3d;
typedef Vector<4, double> Vector4d;

template<class T>
class Vector<2, T, typename std::enable_if<std::is_integral<T>::value>::type> {

};

template<class T>
class Vector<2, T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
public:
    T x, y;

    Vector();
    Vector(T scalar);
    Vector(T x, T y);
    Vector(const Vector<2, T> &other);

    //
    // Const member functions
    //

    T Length() const;
    T LengthSquared() const;
    T Distance(T x, T y) const;
    T Distance(const Vector<2, T> &other) const;
    T DistanceSquared(T x, T y) const;
    T DistanceSquared(const Vector<2, T> &other) const;
    T Dot(T x, T y) const;
    T Dot(const Vector<2, T> &other) const;
    T Determinant(T x, T y) const;
    T Determinant(const Vector<2, T> &other) const;
    T Angle(T x, T y) const;
    T Angle(const Vector<2, T> &other) const;

    Vector<2, T> Normalized() const;
    Vector<2, T> Perpendicular() const;
    Vector<2, T> Min(T x, T y) const;
    Vector<2, T> Min(const Vector<2, T> &other) const;
    Vector<2, T> Max(T x, T y) const;
    Vector<2, T> Max(const Vector<2, T> &other) const;
    Vector<2, T> Lerp(T x, T y, double t) const;
    Vector<2, T> Lerp(const Vector<2, T> &other, double t) const;

    Vector<2, T> operator+(const Vector<2, T> &other) const;
    Vector<2, T> operator-(const Vector<2, T> &other) const;
    Vector<2, T> operator*(const Vector<2, T> &other) const;
    Vector<2, T> operator/(const Vector<2, T> &other) const;

    Vector<2, T> operator+(T scalar) const;
    Vector<2, T> operator-(T scalar) const;
    Vector<2, T> operator*(T scalar) const;
    Vector<2, T> operator/(T scalar) const;

    bool operator==(const Vector<2, T> &other) const;
    bool operator==(T scalar) const;

    //
    // Non-const member functions
    //



    Vector<2, T> &Normalize();
    Vector<2, T> &Zero();
    Vector<2, T> &Negate();
    Vector<2, T> &Floor();
    Vector<2, T> &Ceil();
    Vector<2, T> &Round();

    Vector<2, T> &Set(T x, T y);
    Vector<2, T> &Set(const Vector<2, T> &other);
    Vector<2, T> &Set(T scalar);

    Vector<2, T> &operator=(const Vector<2, T> &other);

    Vector<2, T> &operator+=(const Vector<2, T> &other);
    Vector<2, T> &operator-=(const Vector<2, T> &other);
    Vector<2, T> &operator*=(const Vector<2, T> &other);
    Vector<2, T> &operator/=(const Vector<2, T> &other);

    Vector<2, T> &operator=(T scalar);

    Vector<2, T> &operator+=(T scalar);
    Vector<2, T> &operator-=(T scalar);
    Vector<2, T> &operator*=(T scalar);
    Vector<2, T> &operator/=(T scalar);

};

template<class T>
class Vector<3, T, typename std::enable_if<std::is_integral<T>::value>::type> {

};

template<class T>
class Vector<3, T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
public:
    T x, y, z;

    Vector();
    Vector(T scalar);
    Vector(T x, T y, T z);
    Vector(const Vector<3, T> &other);
    Vector(const Vector<2, T> &v2, T z);

    //
    // Const member functions
    //

    T Length() const;
    T LengthSquared() const;
    T Distance(const Vector<3, T> &other) const;
    T DistanceSquared(const Vector<3, T> &other) const;
    T Dot(const Vector<3, T> &other) const;
    T AngleCos(const Vector<3, T> &other) const;
    T Angle(const Vector<3, T> &other) const;

    Vector<3, T> Normalized() const;
    Vector<3, T> Cross(const Vector<3, T> &other) const;
    Vector<3, T> Min(const Vector<3, T> &other) const;
    Vector<3, T> Max(const Vector<3, T> &other) const;
    Vector<3, T> Lerp(const Vector<3, T> &other, T t) const;

    Vector<3, T> operator+(const Vector<3, T> &other) const;
    Vector<3, T> operator-(const Vector<3, T> &other) const;
    Vector<3, T> operator*(const Vector<3, T> &other) const;
    Vector<3, T> operator/(const Vector<3, T> &other) const;

    Vector<3, T> operator+(T scalar) const;
    Vector<3, T> operator-(T scalar) const;
    Vector<3, T> operator*(T scalar) const;
    Vector<3, T> operator/(T scalar) const;

    bool operator==(const Vector<3, T> &other) const;
    bool operator==(T scalar) const;

    //
    // Non-const member functions
    //

    Vector<3, T> &Normalize();
    Vector<3, T> &Zero();
    Vector<3, T> &Negate();
    Vector<3, T> &Floor();
    Vector<3, T> &Ceil();
    Vector<3, T> &Round();

    Vector<3, T> &Set(T x, T y, T z);
    Vector<3, T> &Set(const Vector<2, T> &v2, T z);
    Vector<3, T> &Set(T scalar);

    Vector<3, T> &operator=(const Vector<3, T> &other);

    Vector<3, T> &operator+=(const Vector<3, T> &other);
    Vector<3, T> &operator-=(const Vector<3, T> &other);
    Vector<3, T> &operator*=(const Vector<3, T> &other);
    Vector<3, T> &operator/=(const Vector<3, T> &other);

    Vector<3, T> &operator=(T scalar);

    Vector<3, T> &operator+=(T scalar);
    Vector<3, T> &operator-=(T scalar);
    Vector<3, T> &operator*=(T scalar);
    Vector<3, T> &operator/=(T scalar);

};

template<class T>
class Vector<4, T, typename std::enable_if<std::is_integral<T>::value>::type> {

};

template<class T>
class Vector<4, T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
public:
    T x, y, z, w;

    Vector();
    Vector(T scalar);
    Vector(T x, T y, T z, T w);
    Vector(const Vector<4, T> &other);
    Vector(const Vector<3, T> &v3, T w);
    Vector(const Vector<2, T> &v2, T z, T w);

    //
    // Const member functions
    //

    T Length() const;
    T LengthSquared() const;
    T Distance(const Vector<4, T> &other) const;
    T DistanceSquared(const Vector<4, T> &other) const;
    T Dot(const Vector<4, T> &other) const;
    T AngleCos(const Vector<4, T> &other) const;
    T Angle(const Vector<4, T> &other) const;

    Vector<4, T> Normalized() const;
    Vector<4, T> Min(const Vector<4, T> &other) const;
    Vector<4, T> Max(const Vector<4, T> &other) const;
    Vector<4, T> Lerp(const Vector<4, T> &other, T t) const;

    Vector<4, T> operator+(const Vector<4, T> &other) const;
    Vector<4, T> operator-(const Vector<4, T> &other) const;
    Vector<4, T> operator*(const Vector<4, T> &other) const;
    Vector<4, T> operator/(const Vector<4, T> &other) const;

    Vector<4, T> operator+(T scalar) const;
    Vector<4, T> operator-(T scalar) const;
    Vector<4, T> operator*(T scalar) const;
    Vector<4, T> operator/(T scalar) const;

    bool operator==(const Vector<4, T> &other) const;
    bool operator==(T scalar) const;

    //
    // Non-const member functions
    //

    Vector<4, T> &Normalize();
    Vector<4, T> &Zero();
    Vector<4, T> &Negate();
    Vector<4, T> &Floor();
    Vector<4, T> &Ceil();
    Vector<4, T> &Round();

    Vector<4, T> &Set(T x, T y, T z, T w);
    Vector<4, T> &Set(const Vector<3, T> &v3, T w);
    Vector<4, T> &Set(const Vector<2, T> &v2, T z, T w);
    Vector<4, T> &Set(T scalar);

    Vector<4, T> &operator=(const Vector<4, T> &other);

    Vector<4, T> &operator+=(const Vector<4, T> &other);
    Vector<4, T> &operator-=(const Vector<4, T> &other);
    Vector<4, T> &operator*=(const Vector<4, T> &other);
    Vector<4, T> &operator/=(const Vector<4, T> &other);

    Vector<4, T> &operator=(T scalar);

    Vector<4, T> &operator+=(T scalar);
    Vector<4, T> &operator-=(T scalar);
    Vector<4, T> &operator*=(T scalar);
    Vector<4, T> &operator/=(T scalar);

};

} //namespace a2d

#endif //A2D_VECTOR_H
