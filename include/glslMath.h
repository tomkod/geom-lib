// GLSL-like math library
// Copyright (C) 2019 Tomasz Dobrowolski
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <cstddef>
#include <cstdint>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef NDEBUG
#define GlslMathUnitTests 0
#else
#define GlslMathUnitTests 1
#endif

namespace glsl_math
{

template <typename T>
T min( T x, T y ) { return (x<y)?x:y; }
template <typename T>
T max( T x, T y ) { return (x>y)?x:y; }

#ifndef WIN32
inline float abs(float x) { return std::abs(x); }
inline double abs(double x) { return std::abs(x); }
#endif

template <typename T>
inline T ufmod(T x, T y) { return x - floor(x / y) * y; }

inline int uimod(int x, int y) { return (x<0) ? y-1 - (-x-1)%y : x%y; }

template <typename T>
T pow2(T x) { return x*x; }
template <typename T>
T pow3(T x) { return x*x*x; }
template <typename T>
T sign(T x)
{
    return (x == 0)
        ? static_cast<T>(0)
        : static_cast<T>(x > 0)*static_cast<T>(2) - static_cast<T>(1);
}

template <typename T, typename TT>
T mix(T x, T y, TT t) { return x + (y - x)*t; }

template <typename T>
T smoothmin(T a, T b, T r)
{
    const T f = max<T>(static_cast<T>(0), static_cast<T>(1) - fabs(b - a) / r);
    return min<T>(a, b) - r*static_cast<T>(.25)*f*f;
}

template <typename T>
T smoothabs(T a, T r) // equivalent to -smoothmin(a, -a, r)
{
    const T f = max<T>(static_cast<T>(0), static_cast<T>(1) - fabs(a + a) / r);
    return fabs(a) + r*static_cast<T>(.25)*f*f;
}

template <typename T> class tvec2;
template <typename T> class tvec3;
template <typename T> class tvec4;
template <typename T> class tmat2;
template <typename T> class tmat3;
template <typename T> class tmat4;

#define VEC2_GET(A, B) \
    tvec2<T> A ## B() const { return tvec2<T>(A, B); }
#define VEC2_GET2(A, B) \
    VEC2_GET(A, B) \
    VEC2_GET(B, A)
#define VEC2_GET22(A, B) \
    VEC2_GET2(A, B) \
    VEC2_GET(A, A) \
    VEC2_GET(B, B)
#define VEC2_GET3(A, B, C) \
    VEC2_GET22(A, B) \
    VEC2_GET2(A, C) \
    VEC2_GET2(B, C) \
    VEC2_GET(C, C)
#define VEC2_GET4(A, B, C, D) \
    VEC2_GET3(A, B, C) \
    VEC2_GET2(A, D) \
    VEC2_GET2(B, D) \
    VEC2_GET2(C, D) \
    VEC2_GET(D, D)

#define VEC3_GET(A, B, C) \
    tvec3<T> A ## B ## C() const { return tvec3<T>(A, B, C); }
#define VEC3_GET2(A, B, C) \
    VEC3_GET(A, B, C) \
    VEC3_GET(A, C, B)
#define VEC3_GET3(A, B, C) \
    VEC3_GET2(A, B, C) \
    VEC3_GET2(B, A, C) \
    VEC3_GET2(C, A, B)
#define VEC3_GET22(A, B) \
    VEC3_GET(A, A, B) \
    VEC3_GET(A, B, A) \
    VEC3_GET(B, A, A)
#define VEC3_GET222(A, B) \
    VEC3_GET22(A, B) \
    VEC3_GET22(B, A)
#define VEC3_GET33(A, B, C) \
    VEC3_GET3(A, B, C) \
    VEC3_GET222(A, B) \
    VEC3_GET222(A, C) \
    VEC3_GET222(B, C) \
    VEC3_GET(A, A, A) \
    VEC3_GET(B, B, B) \
    VEC3_GET(C, C, C)
#define VEC3_GET44(A, B, C, D) \
    VEC3_GET33(A, B, C) \
    VEC3_GET3(A, B, D) \
    VEC3_GET3(A, C, D) \
    VEC3_GET3(B, C, D) \
    VEC3_GET222(A, D) \
    VEC3_GET222(B, D) \
    VEC3_GET222(C, D) \
    VEC3_GET(D, D, D)

#define VEC4_GET(A, B, C, D) \
    tvec4<T> A ## B ## C ## D() const { return tvec4<T>(A, B, C, D); }
#define VEC4_GET2(A, B, C, D) \
    VEC4_GET(A, B, C, D) \
    VEC4_GET(A, B, D, C)
#define VEC4_GET3(A, B, C, D) \
    VEC4_GET2(A, B, C, D) \
    VEC4_GET2(A, C, B, D) \
    VEC4_GET2(A, D, B, C)
#define VEC4_GET4(A, B, C, D) \
    VEC4_GET3(A, B, C, D) \
    VEC4_GET3(B, A, C, D) \
    VEC4_GET3(C, A, B, D) \
    VEC4_GET3(D, A, B, C)
#define VEC4_GET22(A, B, C) \
    VEC4_GET(A, A, B, C) \
    VEC4_GET(A, B, A, C) \
    VEC4_GET(A, B, C, A) \
    VEC4_GET(B, A, A, C) \
    VEC4_GET(B, A, C, A) \
    VEC4_GET(B, C, A, A)
#define VEC4_GET222(A, B, C) \
    VEC4_GET22(A, B, C) \
    VEC4_GET22(A, C, B)
#define VEC4_GET2222(A, B, C, D) \
    VEC4_GET222(A, B, C) \
    VEC4_GET222(A, B, D) \
    VEC4_GET222(A, C, D)
#define VEC4_GET22222(A, B, C, D) \
    VEC4_GET2222(A, B, C, D) \
    VEC4_GET2222(B, A, C, D) \
    VEC4_GET2222(C, A, B, D) \
    VEC4_GET2222(D, A, B, C)
#define VEC4_GET33(A, B) \
    VEC4_GET(A, A, A, B) \
    VEC4_GET(A, A, B, A) \
    VEC4_GET(A, B, A, A) \
    VEC4_GET(B, A, A, A)
#define VEC4_GET333(A, B, C, D) \
    VEC4_GET33(A, B) \
    VEC4_GET33(A, C) \
    VEC4_GET33(A, D) \
    VEC4_GET33(B, C) \
    VEC4_GET33(B, D) \
    VEC4_GET33(C, D)
#define VEC4_GET44(A, B, C, D) \
    VEC4_GET4(A, B, C, D) \
    VEC4_GET22222(A, B, C, D) \
    VEC4_GET333(A, B, C, D) \
    VEC4_GET(A, A, A, A) \
    VEC4_GET(B, B, B, B) \
    VEC4_GET(C, C, C, C) \
    VEC4_GET(D, D, D, D)

#define VEC2_SET(A, B) \
    void set_ ## A ## B(const tvec2<T>& a) { A = a.x; B = a.y; }
#define VEC2_SET2(A, B) \
    VEC2_SET(A, B) \
    VEC2_SET(B, A)
#define VEC2_SET3(A, B, C) \
    VEC2_SET2(A, B) \
    VEC2_SET2(A, C) \
    VEC2_SET2(B, C)
#define VEC2_SET4(A, B, C, D) \
    VEC2_SET3(A, B, C) \
    VEC2_SET2(A, D) \
    VEC2_SET2(B, D) \
    VEC2_SET2(C, D)

#define VEC3_SET(A, B, C) \
    void set_ ## A ## B ## C(const tvec3<T>& a) { A = a.x; B = a.y; C = a.z; }
#define VEC3_SET2(A, B, C) \
    VEC3_SET(A, B, C) \
    VEC3_SET(A, C, B)
#define VEC3_SET3(A, B, C) \
    VEC3_SET2(A, B, C) \
    VEC3_SET2(B, A, C) \
    VEC3_SET2(C, A, B)
#define VEC3_SET4(A, B, C, D) \
    VEC3_SET3(A, B, C) \
    VEC3_SET3(A, B, D) \
    VEC3_SET3(A, C, D) \
    VEC3_SET3(B, C, D)

#define VEC4_SET(A, B, C, D) \
    void set_ ## A ## B ## C ## D(const tvec4<T>& a) { A = a.x; B = a.y; C = a.z; D = a.w; }
#define VEC4_SET2(A, B, C, D) \
    VEC4_SET(A, B, C, D) \
    VEC4_SET(A, B, D, C)
#define VEC4_SET3(A, B, C, D) \
    VEC4_SET2(A, B, C, D) \
    VEC4_SET2(A, C, B, D) \
    VEC4_SET2(A, D, B, C)
#define VEC4_SET4(A, B, C, D) \
    VEC4_SET3(A, B, C, D) \
    VEC4_SET3(B, A, C, D) \
    VEC4_SET3(C, A, B, D) \
    VEC4_SET3(D, A, B, C)

template <typename T>
class tvec2 {
public:
    using value_type = T;
    T x, y;
    tvec2() = default;
    tvec2(const tvec2& v) = default;
    tvec2(T x, T y) : x(x), y(y) { }
    tvec2(T v) : x(v), y(v) { }
    tvec2(const tvec3<T>& v);
    tvec2(const tvec4<T>& v);
    tvec2& operator+=(const tvec2& a) { x += a.x; y += a.y; return *this; }
    tvec2& operator-=(const tvec2& a) { x -= a.x; y -= a.y; return *this; }
    tvec2& operator*=(const tvec2& a) { x *= a.x; y *= a.y; return *this; }
    tvec2& operator/=(const tvec2& a) { x /= a.x; y /= a.y; return *this; }
    tvec2& operator+=(T a) { x += a; y += a; return *this; }
    tvec2& operator-=(T a) { x -= a; y -= a; return *this; }
    tvec2& operator*=(T a) { x *= a; y *= a; return *this; }
    tvec2& operator/=(T a) { x /= a; y /= a; return *this; }

    VEC2_GET22(x, y)
    VEC2_SET2(x, y)

    template <typename TT>
    tvec2(const tvec2<TT>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }
};
template <typename T>
tvec2<T> operator-(const tvec2<T>& a) { return tvec2<T>(-a.x, -a.y); }
template <typename T>
tvec2<T> operator+(const tvec2<T>& a, const tvec2<T>& b) { return tvec2<T>(a.x + b.x, a.y + b.y); }
template <typename T>
tvec2<T> operator-(const tvec2<T>& a, const tvec2<T>& b) { return tvec2<T>(a.x - b.x, a.y - b.y); }
template <typename T>
tvec2<T> operator*(const tvec2<T>& a, const tvec2<T>& b) { return tvec2<T>(a.x * b.x, a.y * b.y); }
template <typename T>
tvec2<T> operator/(const tvec2<T>& a, const tvec2<T>& b) { return tvec2<T>(a.x / b.x, a.y / b.y); }
template <typename T>
tvec2<T> operator+(const tvec2<T>& a, T b) { return tvec2<T>(a.x + b, a.y + b); }
template <typename T>
tvec2<T> operator-(const tvec2<T>& a, T b) { return tvec2<T>(a.x - b, a.y - b); }
template <typename T>
tvec2<T> operator*(const tvec2<T>& a, T b) { return tvec2<T>(a.x * b, a.y * b); }
template <typename T>
tvec2<T> operator*(T b, const tvec2<T>& a) { return tvec2<T>(a.x * b, a.y * b); }
template <typename T>
tvec2<T> operator/(const tvec2<T>& a, T b) { return tvec2<T>(a.x / b, a.y / b); }
template <typename T>
bool operator==(const tvec2<T>& a, const tvec2<T>& b) { return a.x == b.x && a.y == b.y; }
template <typename T>
bool operator!=(const tvec2<T>& a, const tvec2<T>& b) { return a.x != b.x || a.y != b.y; }

template <typename T>
class tvec3 {
public:
    using value_type = T;
    T x, y, z;
    tvec3<T>() = default;
    tvec3<T>(const tvec3<T>& v) = default;
    tvec3<T>(const tvec2<T>& v) : x(v.x), y(v.y), z(0) { }
    tvec3<T>(const tvec2<T>& v, T z) : x(v.x), y(v.y), z(z) { }
    tvec3<T>(T x, const tvec2<T>& v) : x(x), y(v.x), z(v.y) { }
    tvec3<T>(T x, T y, T z) : x(x), y(y), z(z) { }
    tvec3<T>(T v) : x(v), y(v), z(v) { }
    tvec3<T>(const tvec4<T>& v);
    tvec3<T>& operator+=(const tvec3<T>& a) { x += a.x; y += a.y; z += a.z; return *this; }
    tvec3<T>& operator-=(const tvec3<T>& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
    tvec3<T>& operator*=(const tvec3<T>& a) { x *= a.x; y *= a.y; z *= a.z; return *this; }
    tvec3<T>& operator/=(const tvec3<T>& a) { x /= a.x; y /= a.y; z /= a.z; return *this; }
    tvec3<T>& operator+=(T a) { x += a; y += a; z += a; return *this; }
    tvec3<T>& operator-=(T a) { x -= a; y -= a; z -= a; return *this; }
    tvec3<T>& operator*=(T a) { x *= a; y *= a; z *= a; return *this; }
    tvec3<T>& operator/=(T a) { x /= a; y /= a; z /= a; return *this; }

    VEC2_GET3(x, y, z)
    VEC2_SET3(x, y, z)

    VEC3_GET33(x, y, z)
    VEC3_SET3(x, y, z)

    template <typename TT>
    tvec3(const tvec3<TT>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) { }
};
template <typename T>
tvec3<T> operator-(const tvec3<T>& a) { return tvec3<T>(-a.x, -a.y, -a.z); }
template <typename T>
tvec3<T> operator+(const tvec3<T>& a, const tvec3<T>& b) { return tvec3<T>(a.x + b.x, a.y + b.y, a.z + b.z); }
template <typename T>
tvec3<T> operator-(const tvec3<T>& a, const tvec3<T>& b) { return tvec3<T>(a.x - b.x, a.y - b.y, a.z - b.z); }
template <typename T>
tvec3<T> operator*(const tvec3<T>& a, const tvec3<T>& b) { return tvec3<T>(a.x * b.x, a.y * b.y, a.z * b.z); }
template <typename T>
tvec3<T> operator/(const tvec3<T>& a, const tvec3<T>& b) { return tvec3<T>(a.x / b.x, a.y / b.y, a.z / b.z); }
template <typename T>
tvec3<T> operator+(const tvec3<T>& a, T b) { return tvec3<T>(a.x + b, a.y + b, a.z + b); }
template <typename T>
tvec3<T> operator-(const tvec3<T>& a, T b) { return tvec3<T>(a.x - b, a.y - b, a.z - b); }
template <typename T>
tvec3<T> operator*(const tvec3<T>& a, T b) { return tvec3<T>(a.x * b, a.y * b, a.z * b); }
template <typename T>
tvec3<T> operator*(T b, const tvec3<T>& a) { return tvec3<T>(a.x * b, a.y * b, a.z * b); }
template <typename T>
tvec3<T> operator/(const tvec3<T>& a, T b) { return tvec3<T>(a.x / b, a.y / b, a.z / b); }
template <typename T>
bool operator==(const tvec3<T>& a, const tvec3<T>& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
template <typename T>
bool operator!=(const tvec3<T>& a, const tvec3<T>& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

template <typename T>
class tvec4 {
public:
    using value_type = T;
    T x, y, z, w;
    tvec4<T>() = default;
    tvec4<T>(const tvec4<T>& v) = default;
    tvec4<T>(const tvec3<T>& v) : x(v.x), y(v.y), z(v.z), w(1) { }
    tvec4<T>(const tvec2<T>& v) : x(v.x), y(v.y), z(0), w(1) { }
    tvec4<T>(const tvec2<T>& v, const tvec2<T>& u) : x(v.x), y(v.y), z(u.x), w(u.y) { }
    tvec4<T>(const tvec2<T>& v, T z, T w) : x(v.x), y(v.y), z(z), w(w) { }
    tvec4<T>(T x, const tvec2<T>& v, T w) : x(x), y(v.x), z(v.y), w(w) { }
    tvec4<T>(T x, T y, const tvec2<T>& v) : x(x), y(y), z(v.x), w(v.y) { }
    tvec4<T>(const tvec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) { }
    tvec4<T>(T x, const tvec3<T>& v) : x(x), y(v.x), z(v.y), w(v.z) { }
    tvec4<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
    tvec4<T>(T v) : x(v), y(v), z(v), w(v) { }
    tvec4<T>& operator+=(const tvec4<T>& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
    tvec4<T>& operator-=(const tvec4<T>& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
    tvec4<T>& operator*=(const tvec4<T>& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this; }
    tvec4<T>& operator/=(const tvec4<T>& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this; }
    tvec4<T>& operator+=(T a) { x += a; y += a; z += a; w += a; return *this; }
    tvec4<T>& operator-=(T a) { x -= a; y -= a; z -= a; w -= a; return *this; }
    tvec4<T>& operator*=(T a) { x *= a; y *= a; z *= a; w *= a; return *this; }
    tvec4<T>& operator/=(T a) { x /= a; y /= a; z /= a; w /= a; return *this; }

    VEC2_GET4(x, y, z, w)
    VEC2_SET4(x, y, z, w)

    VEC3_GET44(x, y, z, w)
    VEC3_SET4(x, y, z, w)

    VEC4_GET44(x, y, z, w)
    VEC4_SET4(x, y, z, w)

    template <typename TT>
    tvec4(const tvec4<TT>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) { }
};
template <typename T>
tvec4<T> operator-(const tvec4<T>& a) { return tvec4<T>(-a.x, -a.y, -a.z, -a.w); }
template <typename T>
tvec4<T> operator+(const tvec4<T>& a, const tvec4<T>& b) { return tvec4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
template <typename T>
tvec4<T> operator-(const tvec4<T>& a, const tvec4<T>& b) { return tvec4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
template <typename T>
tvec4<T> operator*(const tvec4<T>& a, const tvec4<T>& b) { return tvec4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
template <typename T>
tvec4<T> operator/(const tvec4<T>& a, const tvec4<T>& b) { return tvec4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
template <typename T>
tvec4<T> operator+(const tvec4<T>& a, T b) { return tvec4<T>(a.x + b, a.y + b, a.z + b, a.w + b); }
template <typename T>
tvec4<T> operator-(const tvec4<T>& a, T b) { return tvec4<T>(a.x - b, a.y - b, a.z - b, a.w - b); }
template <typename T>
tvec4<T> operator*(const tvec4<T>& a, T b) { return tvec4<T>(a.x * b, a.y * b, a.z * b, a.w * b); }
template <typename T>
tvec4<T> operator*(T b, const tvec4<T>& a) { return tvec4<T>(a.x * b, a.y * b, a.z * b, a.w * b); }
template <typename T>
tvec4<T> operator/(const tvec4<T>& a, T b) { return tvec4<T>(a.x / b, a.y / b, a.z / b, a.w / b); }
template <typename T>
bool operator==(const tvec4<T>& a, const tvec4<T>& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
template <typename T>
bool operator!=(const tvec4<T>& a, const tvec4<T>& b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

#undef VEC2_GET
#undef VEC2_GET2
#undef VEC2_GET22
#undef VEC2_GET3
#undef VEC2_GET4

#undef VEC3_GET
#undef VEC3_GET2
#undef VEC3_GET3
#undef VEC3_GET22
#undef VEC3_GET222
#undef VEC3_GET33
#undef VEC3_GET44

#undef VEC4_GET2
#undef VEC4_GET3
#undef VEC4_GET4
#undef VEC4_GET22
#undef VEC4_GET222
#undef VEC4_GET2222
#undef VEC4_GET22222
#undef VEC4_GET33
#undef VEC4_GET333
#undef VEC4_GET44

#undef VEC2_SET
#undef VEC2_SET2
#undef VEC2_SET3
#undef VEC2_SET4

#undef VEC3_SET
#undef VEC3_SET2
#undef VEC3_SET3
#undef VEC3_SET4

#undef VEC4_SET
#undef VEC4_SET2
#undef VEC4_SET3
#undef VEC4_SET4

template <typename T>
class tmat2 {
public:
    tvec2<T> m[2];
    tmat2<T>() = default;
    tmat2<T>(const tmat2<T>& m) = default;
    tmat2<T>(const tmat3<T>& m);
    tmat2<T>(const tmat4<T>& m);
    tmat2<T>(T v) : m{{v,0}, {0,v}} { }
    tmat2<T>(T xx, T xy, T yx, T yy) : m{{xx,xy}, {yx,yy}} { }
    tmat2<T>(const tvec2<T>& x, const tvec2<T>& y) : m{x, y} { }
    tvec2<T>& operator[](size_t index) { return m[index]; }
    const tvec2<T>& operator[](size_t index) const { return m[index]; }
    tmat2<T>& operator*=(T s) { m[0] *= s; m[1] *= s; return *this; }
    tmat2<T>& operator*=(const tmat2<T>& a);

    template <typename TT>
    tmat2(const tmat2<TT>& v) : m{v.m[0], v.m[1]} { }
};

template <typename T>
class tmat3 {
public:
    tvec3<T> m[3];
    tmat3<T>() = default;
    tmat3<T>(const tmat3<T>& m) = default;
    tmat3<T>(const tmat4<T>& m);
    tmat3<T>(const tmat2<T>& m) : m{tvec3<T>(m[0], 0), tvec3<T>(m[1], 0), tvec3<T>(0, 0, 1)} { }
    tmat3<T>(T v) : m{{v, 0, 0}, {0, v, 0}, {0, 0, v}} { }
    tmat3<T>(T xx, T xy, T xz,
         T yx, T yy, T yz,
         T zx, T zy, T zz)
        : m{{xx, xy, xz}
        ,   {yx, yy, yz}
        ,   {zx, zy, zz}}
    {
    }
    tmat3<T>(const tvec3<T>& x, const tvec3<T>& y, const tvec3<T>& z) : m{x, y, z} { }
    tvec3<T>& operator[](size_t index) { return m[index]; }
    const tvec3<T>& operator[](size_t index) const { return m[index]; }
    tmat3<T>& operator*=(T s) { m[0] *= s; m[1] *= s; m[2] *= s; return *this; }
    tmat3<T>& operator*=(const tmat3<T>& a);

    template <typename TT>
    tmat3(const tmat3<TT>& v) : m{v.m[0], v.m[1], v.m[2]} { }
};

template <typename T>
class tmat4 {
public:
    tvec4<T> m[4];
    tmat4<T>() = default;
    tmat4<T>(const tmat4<T>& m) = default;
    tmat4<T>(const tmat3<T>& m) : m{tvec4<T>(m[0], 0), tvec4<T>(m[1], 0), tvec4<T>(m[2], 0), tvec4<T>(0, 0, 0, 1)} { }
    tmat4<T>(const tmat2<T>& m) : m{tvec4<T>(m[0], 0, 0), tvec4<T>(m[1], 0, 0), tvec4<T>(0, 0, 1, 0), tvec4<T>(0, 0, 0, 1)} { }
    tmat4<T>(T v) : m{{v, 0, 0, 0}, {0, v, 0, 0}, {0, 0, v, 0}, {0, 0, 0, v}} { }
    tmat4<T>(T xx, T xy, T xz, T xw,
         T yx, T yy, T yz, T yw,
         T zx, T zy, T zz, T zw,
         T wx, T wy, T wz, T ww)
        : m{{xx, xy, xz, xw}
        ,   {yx, yy, yz, yw}
        ,   {zx, zy, zz, zw}
        ,   {wx, wy, wz, ww}}
    {
    }
    tmat4<T>(const tvec4<T>& x, const tvec4<T>& y, const tvec4<T>& z, const tvec4<T>& w) : m{x, y, z, w} { }
    tvec4<T>& operator[](size_t index) { return m[index]; }
    const tvec4<T>& operator[](size_t index) const { return m[index]; }
    tmat4<T>& operator*=(T s) { m[0] *= s; m[1] *= s; m[2] *= s; m[3] *= s; return *this; }
    tmat4<T>& operator*=(const tmat4<T>& a);

    template <typename TT>
    tmat4(const tmat4<TT>& v) : m{v.m[0], v.m[1], v.m[2], v.m[4]} { }
};

static_assert(sizeof(tmat2<float>) == 4 * 4, "invalid tmat2<T> alignment");
static_assert(sizeof(tmat3<float>) == 9 * 4, "invalid tmat3<T> alignment");
static_assert(sizeof(tmat4<float>) == 16 * 4, "invalid tmat4<T> alignment");

static_assert(sizeof(tmat2<double>) == 4 * 8, "invalid tmat2<T> alignment");
static_assert(sizeof(tmat3<double>) == 9 * 8, "invalid tmat3<T> alignment");
static_assert(sizeof(tmat4<double>) == 16 * 8, "invalid tmat4<T> alignment");

template <typename T>
tvec2<T> abs(const tvec2<T>& a) { return tvec2<T>(abs(a.x), abs(a.y)); }
template <typename T>
tvec3<T> abs(const tvec3<T>& a) { return tvec3<T>(abs(a.x), abs(a.y), abs(a.z)); }
template <typename T>
tvec4<T> abs(const tvec4<T>& a) { return tvec4<T>(abs(a.x), abs(a.y), abs(a.z), abs(a.w)); }

template <typename T>
tvec2<T> max(const tvec2<T>& a, const tvec2<T>& b ) { return tvec2<T>(max(a.x,b.x), max(a.y,b.y)); }
template <typename T>
tvec3<T> max(const tvec3<T>& a, const tvec3<T>& b ) { return tvec3<T>(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z)); }
template <typename T>
tvec4<T> max(const tvec4<T>& a, const tvec4<T>& b ) { return tvec4<T>(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z), max(a.w,b.w)); }

template <typename T>
tvec2<T> min(const tvec2<T>& a, const tvec2<T>& b ) { return tvec2<T>(min(a.x,b.x), min(a.y,b.y)); }
template <typename T>
tvec3<T> min(const tvec3<T>& a, const tvec3<T>& b ) { return tvec3<T>(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z)); }
template <typename T>
tvec4<T> min(const tvec4<T>& a, const tvec4<T>& b ) { return tvec4<T>(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z), min(a.w,b.w)); }

inline float floor(float x) { return std::floor(x); }
inline double floor(double x) { return std::floor(x); }
template <typename T>
tvec2<T> floor(const tvec2<T>& v) { return tvec2<T>(floor(v.x), floor(v.y)); }
template <typename T>
tvec3<T> floor(const tvec3<T>& v) { return tvec3<T>(floor(v.x), floor(v.y), floor(v.z)); }
template <typename T>
tvec4<T> floor(const tvec4<T>& v) { return tvec4<T>(floor(v.x), floor(v.y), floor(v.z), floor(v.w)); }

inline float fract(float x) { return x - floor(x); }
inline double fract(double x) { return x - floor(x); }
template <typename T>
tvec2<T> fract(const tvec2<T>& x) { return x - floor(x); }
template <typename T>
tvec3<T> fract(const tvec3<T>& x) { return x - floor(x); }
template <typename T>
tvec4<T> fract(const tvec4<T>& x) { return x - floor(x); }

template <typename T>
tvec2<T> ceil(const tvec2<T>& v) { return tvec2<T>(::ceil(v.x), ::ceil(v.y)); }
template <typename T>
tvec3<T> ceil(const tvec3<T>& v) { return tvec3<T>(::ceil(v.x), ::ceil(v.y), ::ceil(v.z)); }
template <typename T>
tvec4<T> ceil(const tvec4<T>& v) { return tvec4<T>(::ceil(v.x), ::ceil(v.y), ::ceil(v.z), ::ceil(v.w)); }

template <typename T>
T cross(const tvec2<T>& a, const tvec2<T>& b) { return a.x*b.y - a.y*b.x; }
template <typename T>
tvec3<T> cross(const tvec3<T>& a, const tvec3<T>& b) { return tvec3<T>(a.y*b.z - b.y*a.z, a.z*b.x - b.z*a.x, a.x*b.y - b.x*a.y); }
template <typename T>
T dot(const tvec2<T>& a, const tvec2<T>& b) { return a.x*b.x + a.y*b.y; }
template <typename T>
T dot(const tvec3<T>& a, const tvec3<T>& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
template <typename T>
T dot(const tvec4<T>& a, const tvec4<T>& b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

template<typename T>
typename T::value_type length(const T& a) { return sqrt(dot(a, a)); }
template<typename T>
typename T::value_type distance(const T& a, const T& b) { return length(a - b); }
template<typename T>
typename T::value_type squareDistance(const T& a, const T& b) { const T d = a - b; return dot(d, d); }

template<typename T>
T normalize(const T& a)
{
    const auto d = length(a);
    if (d == 0)
        return T(0);
    return a*(static_cast<typename T::value_type>(1.0) / d);
}

template<typename T>
T reflect(const T& a, const T& b)
{
    return a - (static_cast<typename T::value_type>(2)*dot(a, b))*b;
}

template<typename T>
void identity(T& m) { m = T(1.0); }

template <typename T>
tvec2<T>::tvec2(const tvec3<T>& v) : x(v.x), y(v.y) { }
template <typename T>
tvec2<T>::tvec2(const tvec4<T>& v) : x(v.x), y(v.y) { }
template <typename T>
tvec3<T>::tvec3(const tvec4<T>& v) : x(v.x), y(v.y), z(v.z) { }

template <typename T>
tmat2<T>::tmat2(const tmat3<T>& m) : m{{m[0].x, m[0].y}, {m[1].x, m[1].y}} { }
template <typename T>
tmat2<T>::tmat2(const tmat4<T>& m) : m{{m[0].x, m[0].y}, {m[1].x, m[1].y}} { }

template <typename T>
tmat3<T>::tmat3(const tmat4<T>& m)
    : m{{m[0].x, m[0].y, m[0].z}
    ,   {m[1].x, m[1].y, m[1].z}
    ,   {m[2].x, m[2].y, m[2].z}}
{
}

template <typename T>
tvec2<T> operator*(const tmat2<T>& m, const tvec2<T>& v)
{
    return tvec2<T>(
        m[0].x * v.x + m[1].x * v.y,
        m[0].y * v.x + m[1].y * v.y);
}

template <typename T>
tvec2<T> operator*(const tvec2<T>& v, const tmat2<T>& m)
{
    return tvec2<T>(
        m[0].x * v.x + m[0].y * v.y,
        m[1].x * v.x + m[1].y * v.y);
}

template <typename T>
tvec3<T> operator*(const tmat3<T>& m, const tvec3<T>& v)
{
    return tvec3<T>(
        m[0].x * v.x + m[1].x * v.y + m[2].x * v.z,
        m[0].y * v.x + m[1].y * v.y + m[2].y * v.z,
        m[0].z * v.x + m[1].z * v.y + m[2].z * v.z);
}

template <typename T>
tvec3<T> operator*(const tvec3<T>& v, const tmat3<T>& m)
{
    return tvec3<T>(
        m[0].x * v.x + m[0].y * v.y + m[0].z * v.z,
        m[1].x * v.x + m[1].y * v.y + m[1].z * v.z,
        m[2].x * v.x + m[2].y * v.y + m[2].z * v.z);
}

template <typename T>
tvec4<T> operator*(const tmat4<T>& m, const tvec4<T>& v)
{
    return tvec4<T>(
        m[0].x * v.x + m[1].x * v.y + m[2].x * v.z + m[3].x * v.w,
        m[0].y * v.x + m[1].y * v.y + m[2].y * v.z + m[3].y * v.w,
        m[0].z * v.x + m[1].z * v.y + m[2].z * v.z + m[3].z * v.w,
        m[0].w * v.x + m[1].w * v.y + m[2].w * v.z + m[3].w * v.w);
}

template <typename T>
tvec4<T> operator*(const tvec4<T>& v, const tmat4<T>& m)
{
    return tvec4<T>(
        m[0].x * v.x + m[0].y * v.y + m[0].z * v.z + m[0].w * v.w,
        m[1].x * v.x + m[1].y * v.y + m[1].z * v.z + m[1].w * v.w,
        m[2].x * v.x + m[2].y * v.y + m[2].z * v.z + m[2].w * v.w,
        m[3].x * v.x + m[3].y * v.y + m[3].z * v.z + m[3].w * v.w);
}

template <typename T>
tmat2<T> operator*(const tmat2<T>& a, const tmat2<T>& b)
{
    // Multiplication:
    // |00 10| * |00 10| = |0 2|
    // |01 11|   |01 11|   |1 3|
    const T a00 = a[0].x, a01 = a[0].y, a10 = a[1].x, a11 = a[1].y;
    const T b00 = b[0].x, b01 = b[0].y, b10 = b[1].x, b11 = b[1].y;
    return tmat2<T>(
        a00*b00 + a10*b01,
        a01*b00 + a11*b01,
        a00*b10 + a10*b11,
        a01*b10 + a11*b11);
}

template <typename T>
tmat3<T> operator*(const tmat3<T>& a, const tmat3<T>& b)
{
    // Multiplication:
    // |00 10 20|   |00 10 20|   |0 3 6|
    // |01 11 21| * |01 11 21| = |1 4 7|
    // |02 12 22|   |02 12 22|   |2 5 8|
    const T a00 = a[0].x, a01 = a[0].y, a02 = a[0].z;
    const T a10 = a[1].x, a11 = a[1].y, a12 = a[1].z;
    const T a20 = a[2].x, a21 = a[2].y, a22 = a[2].z;
    const T b00 = b[0].x, b01 = b[0].y, b02 = b[0].z;
    const T b10 = b[1].x, b11 = b[1].y, b12 = b[1].z;
    const T b20 = b[2].x, b21 = b[2].y, b22 = b[2].z;
    return tmat3<T>(
        a00*b00 + a10*b01 + a20*b02,
        a01*b00 + a11*b01 + a21*b02,
        a02*b00 + a12*b01 + a22*b02,
        a00*b10 + a10*b11 + a20*b12,
        a01*b10 + a11*b11 + a21*b12,
        a02*b10 + a12*b11 + a22*b12,
        a00*b20 + a10*b21 + a20*b22,
        a01*b20 + a11*b21 + a21*b22,
        a02*b20 + a12*b21 + a22*b22);
}

template <typename T>
tmat4<T> operator*(const tmat4<T>& a, const tmat4<T>& b)
{
    // Multiplication:
    // |00 10 20 30|   |00 10 20 30|   |0 4  8 12|
    // |01 11 21 31| * |01 11 21 31| = |1 5  9 13|
    // |02 12 22 32|   |02 12 22 32|   |2 6 10 14|
    // |03 13 23 33|   |03 13 23 33|   |3 7 11 15|
    const T a00 = a[0].x, a01 = a[0].y, a02 = a[0].z, a03 = a[0].w;
    const T a10 = a[1].x, a11 = a[1].y, a12 = a[1].z, a13 = a[1].w;
    const T a20 = a[2].x, a21 = a[2].y, a22 = a[2].z, a23 = a[2].w;
    const T a30 = a[3].x, a31 = a[3].y, a32 = a[3].z, a33 = a[3].w;
    const T b00 = b[0].x, b01 = b[0].y, b02 = b[0].z, b03 = b[0].w;
    const T b10 = b[1].x, b11 = b[1].y, b12 = b[1].z, b13 = b[1].w;
    const T b20 = b[2].x, b21 = b[2].y, b22 = b[2].z, b23 = b[2].w;
    const T b30 = b[3].x, b31 = b[3].y, b32 = b[3].z, b33 = b[3].w;
    return tmat4<T>(
        a00*b00 + a10*b01 + a20*b02 + a30*b03,
        a01*b00 + a11*b01 + a21*b02 + a31*b03,
        a02*b00 + a12*b01 + a22*b02 + a32*b03,
        a03*b00 + a13*b01 + a23*b02 + a33*b03,
        a00*b10 + a10*b11 + a20*b12 + a30*b13,
        a01*b10 + a11*b11 + a21*b12 + a31*b13,
        a02*b10 + a12*b11 + a22*b12 + a32*b13,
        a03*b10 + a13*b11 + a23*b12 + a33*b13,
        a00*b20 + a10*b21 + a20*b22 + a30*b23,
        a01*b20 + a11*b21 + a21*b22 + a31*b23,
        a02*b20 + a12*b21 + a22*b22 + a32*b23,
        a03*b20 + a13*b21 + a23*b22 + a33*b23,
        a00*b30 + a10*b31 + a20*b32 + a30*b33,
        a01*b30 + a11*b31 + a21*b32 + a31*b33,
        a02*b30 + a12*b31 + a22*b32 + a32*b33,
        a03*b30 + a13*b31 + a23*b32 + a33*b33);
}

template <typename T>
tmat2<T>& tmat2<T>::operator*=(const tmat2<T>& a)
{
    *this = (*this) * a;
    return *this;
}

template <typename T>
tmat3<T>& tmat3<T>::operator*=(const tmat3<T>& a)
{
    *this = (*this) * a;
    return *this;
}

template <typename T>
tmat4<T>& tmat4<T>::operator*=(const tmat4<T>& a)
{
    *this = (*this) * a;
    return *this;
}

template <typename T, typename T1 = T, typename T2 = T>
void translate(tmat3<T>& m, T1 _x, T2 _y)
{
    T x = static_cast<T>(_x);
    T y = static_cast<T>(_y);
    m[2].x += m[0].x * x + m[1].x * y;
    m[2].y += m[0].y * x + m[1].y * y;
}

template <typename T, typename T1 = T, typename T2 = T, typename T3 = T>
void translate(tmat4<T>& m, T1 _x, T2 _y, T3 _z)
{
    T x = static_cast<T>(_x);
    T y = static_cast<T>(_y);
    T z = static_cast<T>(_z);
    m[3].x += m[0].x * x + m[1].x * y + m[2].x * z;
    m[3].y += m[0].y * x + m[1].y * y + m[2].y * z;
    m[3].z += m[0].z * x + m[1].z * y + m[2].z * z;
}

template <typename T>
tmat2<T> rotation(T angle)
{
    T co, si;

    angle *= M_PI / 180.0;
    co = cos(angle);
    si = sin(angle);

    T bm0 = co, bm1 = si,
      bm2 = -si, bm3 = co;

    return tmat2<T>(bm0, bm1, bm2, bm3);
}

template <typename T>
tmat3<T> rotation(T angle, T ax, T ay, T az)
{
    T si, co, ti, tx, ty, tz, sx, sy, sz;

    T ad = sqrt(ax*ax + ay*ay + az*az);
    if (ad > 0) {
        ad = 1 / ad;
        ax *= ad;
        ay *= ad;
        az *= ad;
    }
    else {
        return tmat3<T>(1);
    }

    angle *= -M_PI / 180.0;
    si = sin(angle);
    co = cos(angle);
    ti = 1 - co;

    tx = ti * ax; ty = ti * ay; tz = ti * az;
    sx = si * ax; sy = si * ay; sz = si * az;

    // derived from multiply(mm,mm,bm)
    T bm0, bm1, bm2, bm4, bm5, bm6, bm8, bm9, bm10;

    bm0 = tx * ax + co;
    bm4 = tx * ay + sz;
    bm8 = tx * az - sy;

    bm1 = tx * ay - sz;
    bm5 = ty * ay + co;
    bm9 = ty * az + sx;

    bm2 = tx * az + sy;
    bm6 = ty * az - sx;
    bm10 = tz * az + co;

    return tmat3<T>(
        bm0, bm1, bm2,
        bm4, bm5, bm6,
        bm8, bm9, bm10);
}

template <typename T, typename T1 = T>
void rotate(tmat3<T>& m, T1 angle)
{
    m *= tmat3<T>(rotation<T>(static_cast<T>(angle)));
}

template <typename T, typename T1 = T, typename T2 = T, typename T3 = T, typename T4 = T>
void rotate(tmat4<T>& m, T1 angle, T2 ax, T3 ay, T4 az)
{
    m *= tmat4<T>(rotation<T>(static_cast<T>(angle), static_cast<T>(ax), static_cast<T>(ay), static_cast<T>(az)));
}

template <typename T>
tmat2<T> transpose(const tmat2<T>& m)
{
    return tmat2<T>(
        m[0].x, m[1].x,
        m[0].y, m[1].y);
}

template <typename T>
tmat3<T> transpose(const tmat3<T>& m)
{
    return tmat3<T>(
        m[0].x, m[1].x, m[2].x,
        m[0].y, m[1].y, m[2].y,
        m[0].z, m[1].z, m[2].z);
}

template <typename T>
tmat4<T> transpose(const tmat4<T>& m)
{
    return tmat4<T>(
        m[0].x, m[1].x, m[2].x, m[3].x,
        m[0].y, m[1].y, m[2].y, m[3].y,
        m[0].z, m[1].z, m[2].z, m[3].z,
        m[0].w, m[1].w, m[2].w, m[3].w);
}

template <typename T>
tmat2<T> inverse(const tmat2<T>& m)
{
    // column-major order:
    // |a c|
    // |b d|
    const T a = m[0].x, b = m[0].y, c = m[1].x, d = m[1].y;
    const T f = 1.0 / (a*d - b*c);
    return tmat2<T>(d*f, -b*f, -c*f, a*f);
}

template <typename T>
tmat3<T> inverse(const tmat3<T>& m)
{
    // column-major order:
    // | ax bx cx | ax bx cx
    // | ay by cy | ay by cy
    // | az bz cz | az bz cz
    // +----------+----------
    // | ax bx cx | ax bx cx
    // | ay by cy | ay by cy
    // | az bz cz | az bz cz
    // co-factor:
    // | by*cz-bz*cy cy*az-cz*ay ay*bz-az*by |
    // | bz*cx-bx*cz cz*ax-cx*az az*bx-ax*bz |
    // | bx*cy-by*cx cx*ay-cy*ax ax*by-ay*bx |
    const T ax = m[0].x, ay = m[0].y, az = m[0].z;
    const T bx = m[1].x, by = m[1].y, bz = m[1].z;
    const T cx = m[2].x, cy = m[2].y, cz = m[2].z;
    const T dax = by*cz - bz*cy;
    const T dbx = cy*az - cz*ay;
    const T dcx = ay*bz - az*by;
    const T d = 1.0 / (ax*dax + bx*dbx + cx*dcx);
    // transpose of co-factor matrix multipled by 1/det
    // in column-major order
    return tmat3<T>(dax*d, dbx*d, dcx*d,
            (bz*cx - bx*cz)*d, (cz*ax - cx*az)*d, (az*bx - ax*bz)*d,
            (bx*cy - by*cx)*d, (cx*ay - cy*ax)*d, (ax*by - ay*bx)*d);
}

template <typename T>
tmat4<T> inverse(const tmat4<T>& m) {
    T tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11;
    T src0, src1, src2, src3, src4, src5, src6, src7, src8, src9, src10, src11, src12, src13, src14, src15;
    T dst0, dst1, dst2, dst3, dst4, dst5, dst6, dst7, dst8, dst9, dst10, dst11, dst12, dst13, dst14, dst15;

    // Transpose matrix
    src0 = m[0].x;  src1 = m[1].x;  src2 = m[2].x;  src3 = m[3].x;
    src4 = m[0].y;  src5 = m[1].y;  src6 = m[2].y;  src7 = m[3].y;
    src8 = m[0].z;  src9 = m[1].z;  src10 = m[2].z; src11 = m[3].z;
    src12 = m[0].w; src13 = m[1].w; src14 = m[2].w; src15 = m[3].w;

    // Calculate pairs for first 8 elements (cofactors)
    tmp0 = src10 * src15;
    tmp1 = src11 * src14;
    tmp2 = src9  * src15;
    tmp3 = src11 * src13;
    tmp4 = src9  * src14;
    tmp5 = src10 * src13;
    tmp6 = src8  * src15;
    tmp7 = src11 * src12;
    tmp8 = src8  * src14;
    tmp9 = src10 * src12;
    tmp10 = src8 * src13;
    tmp11 = src9 * src12;

    // Calculate first 8 elements (cofactors)
    dst0 = tmp0*src5 + tmp3*src6 + tmp4*src7;
    dst0 -= tmp1*src5 + tmp2*src6 + tmp5*src7;
    dst1 = tmp1*src4 + tmp6*src6 + tmp9*src7;
    dst1 -= tmp0*src4 + tmp7*src6 + tmp8*src7;
    dst2 = tmp2*src4 + tmp7*src5 + tmp10*src7;
    dst2 -= tmp3*src4 + tmp6*src5 + tmp11*src7;
    dst3 = tmp5*src4 + tmp8*src5 + tmp11*src6;
    dst3 -= tmp4*src4 + tmp9*src5 + tmp10*src6;
    dst4 = tmp1*src1 + tmp2*src2 + tmp5*src3;
    dst4 -= tmp0*src1 + tmp3*src2 + tmp4*src3;
    dst5 = tmp0*src0 + tmp7*src2 + tmp8*src3;
    dst5 -= tmp1*src0 + tmp6*src2 + tmp9*src3;
    dst6 = tmp3*src0 + tmp6*src1 + tmp11*src3;
    dst6 -= tmp2*src0 + tmp7*src1 + tmp10*src3;
    dst7 = tmp4*src0 + tmp9*src1 + tmp10*src2;
    dst7 -= tmp5*src0 + tmp8*src1 + tmp11*src2;

    // Calculate pairs for second 8 elements (cofactors)
    tmp0 = src2*src7;
    tmp1 = src3*src6;
    tmp2 = src1*src7;
    tmp3 = src3*src5;
    tmp4 = src1*src6;
    tmp5 = src2*src5;
    tmp6 = src0*src7;
    tmp7 = src3*src4;
    tmp8 = src0*src6;
    tmp9 = src2*src4;
    tmp10 = src0*src5;
    tmp11 = src1*src4;

    // Calculate second 8 elements (cofactors)
    dst8 = tmp0 * src13 + tmp3 * src14 + tmp4 * src15;
    dst8 -= tmp1 * src13 + tmp2 * src14 + tmp5 * src15;
    dst9 = tmp1 * src12 + tmp6 * src14 + tmp9 * src15;
    dst9 -= tmp0 * src12 + tmp7 * src14 + tmp8 * src15;
    dst10 = tmp2 * src12 + tmp7 * src13 + tmp10* src15;
    dst10 -= tmp3 * src12 + tmp6 * src13 + tmp11* src15;
    dst11 = tmp5 * src12 + tmp8 * src13 + tmp11* src14;
    dst11 -= tmp4 * src12 + tmp9 * src13 + tmp10* src14;
    dst12 = tmp2 * src10 + tmp5 * src11 + tmp1 * src9;
    dst12 -= tmp4 * src11 + tmp0 * src9 + tmp3 * src10;
    dst13 = tmp8 * src11 + tmp0 * src8 + tmp7 * src10;
    dst13 -= tmp6 * src10 + tmp9 * src11 + tmp1 * src8;
    dst14 = tmp6 * src9 + tmp11* src11 + tmp3 * src8;
    dst14 -= tmp10* src11 + tmp2 * src8 + tmp7 * src9;
    dst15 = tmp10* src10 + tmp4 * src8 + tmp9 * src9;
    dst15 -= tmp8 * src9 + tmp11* src10 + tmp5 * src8;

    // Multiply by inverse of determinant
    T d = 1.0 / (src0*dst0 + src1*dst1 + src2*dst2 + src3*dst3);
    return tmat4<T>(dst0*d, dst1*d, dst2*d, dst3*d,
            dst4*d, dst5*d, dst6*d, dst7*d,
            dst8*d, dst9*d, dst10*d, dst11*d,
            dst12*d, dst13*d, dst14*d, dst15*d);
}

template <typename T>
void lookAt(tmat4<T>& mat, T px, T py, T pz, T fx, T fy, T fz, T ux, T uy, T uz)
{
    T t, r0, r1, r2, d0, d1, d2, f0, f1, f2;

    f0 = px-fx; f1 = py-fy; f2 = pz-fz;
    t = 1.0/sqrt(f0*f0 + f1*f1 + f2*f2); f0 *= t; f1 *= t; f2 *= t;

    r0 = f2*uy - f1*uz;
    r1 = f0*uz - f2*ux;
    r2 = f1*ux - f0*uy;
    t = 1.0/sqrt(r0*r0 + r1*r1 + r2*r2); r0 *= t; r1 *= t; r2 *= t;

    d0 = f1*r2 - f2*r1;
    d1 = f2*r0 - f0*r2;
    d2 = f0*r1 - f1*r0;

    mat[0].x = r0; mat[1].x = r1; mat[2].x = r2; mat[3].x = -(r0*px + r1*py + r2*pz);
    mat[0].y = d0; mat[1].y = d1; mat[2].y = d2; mat[3].y = -(d0*px + d1*py + d2*pz);
    mat[0].z = f0; mat[1].z = f1; mat[2].z = f2; mat[3].z = -(f0*px + f1*py + f2*pz);
    mat[0].w = 0.0; mat[1].w = 0.0; mat[2].w =  0.0; mat[3].w = 1.0;
}

template <typename T>
void lookAt(tmat4<T>& mat, const tvec3<T>& p, const tvec3<T>& f, const tvec3<T>& u)
{
    lookAt(mat, p.x, p.y, p.z, f.x, f.y, f.z, u.x, u.y, u.z);
}

template <typename T>
void slerp(tmat3<T>& ret, const tmat3<T>& a, const tmat3<T>& b, T rat)
{
    // Assumption is a and b are orthonormal.
    tvec3<T> ax;
    T c, s, t, ox, oy, oz, k[9];

    tvec3<T> irig, idow, ifor, irig2, idow2, ifor2;
    irig = a[0];
    idow = a[1];
    ifor = a[2];
    irig2 = b[0];
    idow2 = b[1];
    ifor2 = b[2];
    ret = a;

    ax.x = irig.y*irig2.z - irig.z*irig2.y + idow.y*idow2.z - idow.z*idow2.y + ifor.y*ifor2.z - ifor.z*ifor2.y;
    ax.y = irig.z*irig2.x - irig.x*irig2.z + idow.z*idow2.x - idow.x*idow2.z + ifor.z*ifor2.x - ifor.x*ifor2.z;
    ax.z = irig.x*irig2.y - irig.y*irig2.x + idow.x*idow2.y - idow.y*idow2.x + ifor.x*ifor2.y - ifor.y*ifor2.x;
    t = ax.x*ax.x + ax.y*ax.y + ax.z*ax.z; if (t == 0) return;

    //Based on the vector suck-out method (see ROTATE2.BAS) by Ken Silverman
    ox = irig.x*ax.x + irig.y*ax.y + irig.z*ax.z;
    oy = idow.x*ax.x + idow.y*ax.y + idow.z*ax.z;
    if (fabs(ox) < fabs(oy))
    {
        c = irig.x*irig2.x + irig.y*irig2.y + irig.z*irig2.z; s = ox*ox;
    }
    else
    {
        c = idow.x*idow2.x + idow.y*idow2.y + idow.z*idow2.z; s = oy*oy;
    }
    if (t == s) return;
    c = (c*t - s) / (t - s);
    if (c < -1) c = -1;
    if (c > 1) c = 1;
    c = acos(c)*rat; s = sin(c); c = cos(c);

    t = 1.0 / sqrt(t); ax.x *= t; ax.y *= t; ax.z *= t;

    t = 1.0f - c;
    k[0] = ax.x*t; k[7] = ax.x*s; oz = ax.y*k[0];
    k[4] = ax.y*t; k[2] = ax.y*s; oy = ax.z*k[0];
    k[8] = ax.z*t; k[3] = ax.z*s; ox = ax.z*k[4];
    k[0] = ax.x*k[0] + c; k[5] = ox - k[7]; k[7] += ox;
    k[4] = ax.y*k[4] + c; k[6] = oy - k[2]; k[2] += oy;
    k[8] = ax.z*k[8] + c; k[1] = oz - k[3]; k[3] += oz;

    ox = ret[0].x; oy = ret[0].y; oz = ret[0].z;
    ret[0].x = ox*k[0] + oy*k[1] + oz*k[2];
    ret[0].y = ox*k[3] + oy*k[4] + oz*k[5];
    ret[0].z = ox*k[6] + oy*k[7] + oz*k[8];

    ox = ret[1].x; oy = ret[1].y; oz = ret[1].z;
    ret[1].x = ox*k[0] + oy*k[1] + oz*k[2];
    ret[1].y = ox*k[3] + oy*k[4] + oz*k[5];
    ret[1].z = ox*k[6] + oy*k[7] + oz*k[8];

    ox = ret[2].x; oy = ret[2].y; oz = ret[2].z;
    ret[2].x = ox*k[0] + oy*k[1] + oz*k[2];
    ret[2].y = ox*k[3] + oy*k[4] + oz*k[5];
    ret[2].z = ox*k[6] + oy*k[7] + oz*k[8];
}

template <typename T>
void slerp(tmat4<T>& ret, const tmat4<T>& a, const tmat4<T>& b, T t)
{
    // Assumption is a and b are orthonormal.
    const tvec3<T> apos = getCameraPosition(a);
    const tvec3<T> bpos = getCameraPosition(b);
    tmat3<T> m;
    slerp(m, tmat3<T>(a), tmat3<T>(b), t);
    ret[0] = tvec4<T>(m[0], 0.0);
    ret[1] = tvec4<T>(m[1], 0.0);
    ret[2] = tvec4<T>(m[2], 0.0);
    // linearly interpolate position
    setCameraPosition(ret, mix(apos, bpos, t));
}

template <typename T>
void orthonormalize(tvec3<T>& v0, tvec3<T>& v1, tvec3<T>& v2)
{
    T t = 1.0 / sqrt((v0.x)*(v0.x) + (v0.y)*(v0.y) + (v0.z)*(v0.z));
    (v0.x) *= t; (v0.y) *= t; (v0.z) *= t;
    t = (v1.x)*(v0.x) + (v1.y)*(v0.y) + (v1.z)*(v0.z);
    (v1.x) -= t*(v0.x); (v1.y) -= t*(v0.y); (v1.z) -= t*(v0.z);
    t = 1.0 / sqrt((v1.x)*(v1.x) + (v1.y)*(v1.y) + (v1.z)*(v1.z));
    (v1.x) *= t; (v1.y) *= t; (v1.z) *= t;
    (v2.x) = (v0.y)*(v1.z) - (v0.z)*(v1.y);
    (v2.y) = (v0.z)*(v1.x) - (v0.x)*(v1.z);
    (v2.z) = (v0.x)*(v1.y) - (v0.y)*(v1.x);
}

template <typename T>
void orthonormalize(tmat3<T>& m)
{
    orthonormalize(m[0], m[1], m[2]);
}

template <typename T>
void orthonormalize(tmat4<T>& m)
{
    tvec3<T> v0 = m[0], v1 = m[1], v2 = m[2];
    orthonormalize(v0, v1, v2);
    m[0] = tvec4<T>(v0, m[0].w);
    m[1] = tvec4<T>(v1, m[1].w);
    m[2] = tvec4<T>(v2, m[2].w);
}

template <typename T>
tvec3<T> barycentric(const tvec2<T>& v, const tvec2<T>& p0, const tvec2<T>& p1, const tvec2<T>& p2)
{
    const tvec2<T> d0 = p0 - v;
    const tvec2<T> d1 = p1 - v;
    const tvec2<T> d2 = p2 - v;
    const T b01 = cross(d1, d0);
    const T b12 = cross(d2, d1);
    const T b20 = cross(d0, d2);
    return tvec3<T>(b12, b20, b01) * (1.0 / (b01 + b12 + b20));
}

template <typename T>
tvec3<T> getCameraPosition(const tmat4<T>& m)
{
    // Derivation:
    //  v' = m*v (homogeneous transformation of vector v)
    //  m[3] = m*(-pos)
    //  pos = -inverse(m)*m[3]
    //  (assuming m is sorthonomal)
    //  pos = -transpose(m)*m[3]
    //  pos = -m[3]*m
    //return -tvec3<T>(m[3])*tmat3<T>(m);
    return inverse(tmat3<T>(m))*(-tvec3<T>(m[3]));
}

template <typename T>
void setCameraPosition(tmat4<T>& m, const tvec3<T>& pos)
{
    m[3] = tvec4<T>(tmat3<T>(m)*(-pos), 1.0);
}

template <typename T>
tmat4<T> perspectiveProjection(T fovy, T aspect, T near_z, T far_z)
{
    const T d = 1.0 / tan(M_PI * fovy / 360.0);
    const T ax = d / aspect;
    const T by = d;
    const T cz = (near_z + far_z) / (near_z - far_z);
    const T cp = 2.0 * far_z * near_z / (near_z - far_z);
    return tmat4<T>(
        ax, 0, 0, 0,
        0, by, 0, 0,
        0, 0, cz, -1,
        0, 0, cp, 0);
}

template <typename T>
tmat4<T> orthographicProjection(T left, T right, T bottom, T top, T near, T far)
{
    const T tx = -(right + left) / (right - left);
    const T ty = -(top + bottom) / (top - bottom);
    const T tz = -(far + near) / (far - near);
    const T ax = 2.0 / (right - left);
    const T by = 2.0 / (top - bottom);
    const T cz = -2.0 / (far - near);
    return tmat4<T>(
        ax, 0, 0, 0,
        0, by, 0, 0,
        0, 0, cz, 0,
        tx, ty, tz, 1);
}

template <typename T>
void calculate_ray(tvec3<T>& ret_pos, tvec3<T>& ret_dir, T screen_x, T screen_y,
    const tmat4<T>& modelView, const tmat4<T>& projection, int viewX, int viewY, int viewWidth, int viewHeight)
{
    // Transformation pipeline:
    //  local (mesh) coords
    //    -> modelView -> view coords
    //    -> projection -> clip coords
    //    -> divide by w -> ndc coords
    //    -> view transformation -> screen coords

    // View transformation
    //  screen_x = (ndc_x * 0.5 + 0.5) * viewWidth + viewX
    //  screen_y = (ndc_y * 0.5 + 0.5) * viewHeight + viewY
    //  screen_z = ndc_z * 0.5 + 0.5

    // Inverse view transformation:
    //  ndc_x = (screen_x - viewX) * 2 / viewWidth - 1
    //  ndc_y = (screen_y - viewY) * 2 / viewHeight - 1
    //  ndc_z = screen_z * 2 - 1

    // Projection transformation:
    //  ndc = view.xyz / view.w
    //  view.xyz = ndc * view.w
    ret_pos = getCameraPosition(modelView);
    tvec4<T> clip_dir = tvec4<T>(
        (screen_x - viewX) * 2 / static_cast<T>(viewWidth) - 1,
        (viewY + viewHeight - screen_y) * 2 / static_cast<T>(viewHeight) - 1,
        1, 1);
    ret_dir = normalize(inverse(modelView) * (inverse(projection) * clip_dir));
}

// Conversions.
template<typename T, typename TT>
void convert(const tmat2<T>& m, TT ret[4]) {
    for(int j = 0, i = 0; i < 4; ++j, i += 2) {
        ret[i] = static_cast<TT>(m[j].x);
        ret[i + 1] = static_cast<TT>(m[j].y);
    }
}
template<typename T, typename TT>
void convert(const tmat3<T>& m, TT ret[9]) {
    for(int j = 0, i = 0; i < 9; ++j, i += 3) {
        ret[i] = static_cast<TT>(m[j].x);
        ret[i + 1] = static_cast<TT>(m[j].y);
        ret[i + 2] = static_cast<TT>(m[j].z);
    }
}
template<typename T, typename TT>
void convert(const tmat4<T>& m, TT ret[16]) {
    for(int j = 0, i = 0; i < 16; ++j, i += 4) {
        ret[i] = static_cast<TT>(m[j].x);
        ret[i + 1] = static_cast<TT>(m[j].y);
        ret[i + 2] = static_cast<TT>(m[j].z);
        ret[i + 3] = static_cast<TT>(m[j].w);
    }
}

using vec2 = tvec2<double>;
using vec3 = tvec3<double>;
using vec4 = tvec4<double>;
using mat2 = tmat2<double>;
using mat3 = tmat3<double>;
using mat4 = tmat4<double>;
using vec2f = tvec2<float>;
using vec3f = tvec3<float>;
using vec4f = tvec4<float>;
using mat2f = tmat2<float>;
using mat3f = tmat3<float>;
using mat4f = tmat4<float>;
using ivec2 = tvec2<int>;
using ivec3 = tvec3<int>;
using ivec4 = tvec4<int>;

#if GlslMathUnitTests
void runUnitTests();
#endif

} // namespace glsl_math
