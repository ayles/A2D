//
// Created by selya on 26.10.2018.
//

#ifndef A2D_VECTOR_HPP
#define A2D_VECTOR_HPP

#include <a2d/math/math_defines.hpp>
#include <a2d/utils/utils.hpp>

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
typedef Vector<2, unsigned int> Vector2ui;
typedef Vector<3, unsigned int> Vector3ui;
typedef Vector<4, unsigned int> Vector4ui;

template<class T>
class Vector<2, T> {
public:
    T x, y;

    Vector();
    Vector(T scalar);
    Vector(T x, T y);
    Vector(const Vector<2, T> &other);

    //
    // Const member functions
    //

    // Unsafe for integer vectors
    T Length() const;
    T Distance(T x, T y) const;
    T Distance(const Vector<2, T> &other) const;
    T Dot(T x, T y) const;
    T Dot(const Vector<2, T> &other) const;
    T Determinant(T x, T y) const;
    T Determinant(const Vector<2, T> &other) const;
    T Angle(T x, T y) const;
    T Angle(const Vector<2, T> &other) const;
    Vector<2, T> Normalized() const;

    T LengthSquared() const;
    T DistanceSquared(T x, T y) const;
    T DistanceSquared(const Vector<2, T> &other) const;

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


    // Unsafe for integer vectors
    Vector<2, T> &Normalize();
    Vector<2, T> &Floor();
    Vector<2, T> &Ceil();
    Vector<2, T> &Round();

    Vector<2, T> &Zero();
    Vector<2, T> &Set(T x, T y);
    Vector<2, T> &Set(const Vector<2, T> &other);
    Vector<2, T> &Set(T scalar);
    Vector<2, T> &operator-();
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
class Vector<3, T> {
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

    // Unsafe for integer vectors
    T Length() const;
    T Distance(const Vector<3, T> &other) const;
    T Distance(T x, T y, T z) const;
    T Dot(const Vector<3, T> &other) const;
    T AngleCos(const Vector<3, T> &other) const;
    T Angle(const Vector<3, T> &other) const;
    Vector<3, T> Normalized() const;

    T LengthSquared() const;
    T DistanceSquared(const Vector<3, T> &other) const;

    Vector<3, T> Cross(const Vector<3, T> &other) const;
    Vector<3, T> Min(const Vector<3, T> &other) const;
    Vector<3, T> Max(const Vector<3, T> &other) const;
    Vector<3, T> Lerp(const Vector<3, T> &other, double t) const;

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

    // Unsafe for integer vectors
    Vector<3, T> &Normalize();
    Vector<3, T> &Floor();
    Vector<3, T> &Ceil();
    Vector<3, T> &Round();

    Vector<3, T> &Zero();
    Vector<3, T> &Set(T x, T y, T z);
    Vector<3, T> &Set(const a2d::Vector<3, T> &other);
    Vector<3, T> &Set(const a2d::Vector<2, T> &other, T z);
    Vector<3, T> &Set(T scalar);
    Vector<3, T> &operator-();
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
class Vector<4, T> {
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

    // Unsafe for integer vectors
    T Length() const;
    T Distance(const Vector<4, T> &other) const;
    T AngleCos(const Vector<4, T> &other) const;
    T Angle(const Vector<4, T> &other) const;
    Vector<4, T> Normalized() const;

    T LengthSquared() const;
    T DistanceSquared(const Vector<4, T> &other) const;
    T Dot(const Vector<4, T> &other) const;

    Vector<4, T> Min(const Vector<4, T> &other) const;
    Vector<4, T> Max(const Vector<4, T> &other) const;
    Vector<4, T> Lerp(const Vector<4, T> &other, double t) const;

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

    // Unsafe for integer vectors
    Vector<4, T> &Normalize();
    Vector<4, T> &Floor();
    Vector<4, T> &Ceil();
    Vector<4, T> &Round();

    Vector<4, T> &Zero();
    Vector<4, T> &Set(T x, T y, T z, T w);
    Vector<4, T> &Set(const a2d::Vector<4, T> &other);
    Vector<4, T> &Set(const a2d::Vector<3, T> &other, T w);
    Vector<4, T> &Set(const a2d::Vector<2, T> &other, T z, T w);
    Vector<4, T> &Set(T scalar);
    Vector<4, T> &operator-();
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

namespace std {

template<class K>
struct hash<a2d::Vector<2, K>> {
    size_t operator()(const a2d::Vector<2, K> &v) const {
        std::hash<K> hasher;
        size_t hash_ = hasher(v.x);
        a2d::utils::hash_combine(hash_, v.y);
        return hash_;
    }
};

template<class K>
struct hash<a2d::Vector<3, K>> {
    size_t operator()(const a2d::Vector<3, K> &v) const {
        std::hash<K> hasher;
        size_t hash_ = hasher(v.x);
        a2d::utils::hash_combine(hash_, v.y);
        a2d::utils::hash_combine(hash_, v.z);
        return hash_;
    }
};

template<class K>
struct hash<a2d::Vector<4, K>> {
    size_t operator()(const a2d::Vector<4, K> &v) const {
        std::hash<K> hasher;
        size_t hash_ = hasher(v.x);
        a2d::utils::hash_combine(hash_, v.y);
        a2d::utils::hash_combine(hash_, v.z);
        a2d::utils::hash_combine(hash_, v.w);
        return hash_;
    }
};

}

#endif //A2D_VECTOR_HPP
