#ifndef GLADOS_VEC3_H
#define GLADOS_VEC3_H

#include "Angle.hpp"
#include "Vec2.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class Vec4;
    class UVec3;
    class Vec3 {
      public:
        Vec3();
        ~Vec3() = default;
        explicit Vec3(real _xyz);
        Vec3(real _x, real _y);
        Vec3(real _x, real _y, real _z);
        explicit Vec3(const Vec2& other);
        explicit Vec3(const Vec4& other);
        Vec3(const Vec2& other, real _z);
        Vec3(Vec3&& other) noexcept;
        Vec3(const Vec3& other) = default;
        Vec3& operator=(Vec3 other);  // copy and swap idiom

        real& operator[](unsigned int i);
        const real& operator[](unsigned int i) const;

        Vec3 operator+(const Vec3& other) const;
        Vec3& operator+=(const Vec3& other);

        Vec3 operator-(const Vec3& other) const;
        Vec3& operator-=(const Vec3& other);

        Vec3 operator*(const Vec3& other) const;
        Vec3& operator*=(const Vec3& other);

        Vec3 operator/(const Vec3& other) const;
        Vec3& operator/=(const Vec3& other);

        Vec3 operator+(real scalar) const;
        Vec3& operator+=(real scalar);

        Vec3 operator-(real scalar) const;
        Vec3& operator-=(real scalar);

        Vec3 operator*(real scalar) const;
        Vec3& operator*=(real scalar);

        Vec3 operator/(real scalar) const;
        Vec3& operator/=(real scalar);

        bool operator==(const Vec3& other) const;
        bool operator!=(const Vec3& other) const;

        Vec3 operator-() const;
        Vec3 operator+() const;
        Vec3& makeNegate();

        Vec3 perpendicular() const;
        UVec3 makeNormalize();
        real length() const;
        real squaredLength() const;
        real distance(const Vec3& other) const;
        real distanceSquare(const Vec3& other) const;

        [[nodiscard]] Vec2 xx() const { return Vec2(x, x); }
        [[nodiscard]] Vec2 xy() const { return Vec2(x, y); }
        [[nodiscard]] Vec2 xz() const { return Vec2(x, z); }
        [[nodiscard]] Vec2 yx() const { return Vec2(y, x); }
        [[nodiscard]] Vec2 yy() const { return Vec2(y, y); }
        [[nodiscard]] Vec2 yz() const { return Vec2(y, z); }
        [[nodiscard]] Vec2 zx() const { return Vec2(z, x); }
        [[nodiscard]] Vec2 zy() const { return Vec2(z, y); }
        [[nodiscard]] Vec2 zz() const { return Vec2(z, z); }

        [[nodiscard]] Vec3 xxx() const { return Vec3(x, x, x); }
        [[nodiscard]] Vec3 xxy() const { return Vec3(x, x, y); }
        [[nodiscard]] Vec3 xxz() const { return Vec3(x, x, z); }
        [[nodiscard]] Vec3 xyx() const { return Vec3(x, y, x); }
        [[nodiscard]] Vec3 xyy() const { return Vec3(x, y, y); }
        [[nodiscard]] Vec3 xyz() const { return Vec3(x, y, z); }
        [[nodiscard]] Vec3 xzx() const { return Vec3(x, z, x); }
        [[nodiscard]] Vec3 xzy() const { return Vec3(x, z, y); }
        [[nodiscard]] Vec3 xzz() const { return Vec3(x, z, z); }

        [[nodiscard]] Vec3 yxx() const { return Vec3(y, x, x); }
        [[nodiscard]] Vec3 yxy() const { return Vec3(y, x, y); }
        [[nodiscard]] Vec3 yxz() const { return Vec3(y, x, z); }
        [[nodiscard]] Vec3 yyx() const { return Vec3(y, y, x); }
        [[nodiscard]] Vec3 yyy() const { return Vec3(y, y, y); }
        [[nodiscard]] Vec3 yyz() const { return Vec3(y, y, z); }
        [[nodiscard]] Vec3 yzx() const { return Vec3(y, z, x); }
        [[nodiscard]] Vec3 yzy() const { return Vec3(y, z, y); }
        [[nodiscard]] Vec3 yzz() const { return Vec3(y, z, z); }

        [[nodiscard]] Vec3 zxx() const { return Vec3(z, x, x); }
        [[nodiscard]] Vec3 zxy() const { return Vec3(z, x, y); }
        [[nodiscard]] Vec3 zxz() const { return Vec3(z, x, z); }
        [[nodiscard]] Vec3 zyx() const { return Vec3(z, y, x); }
        [[nodiscard]] Vec3 zyy() const { return Vec3(z, y, y); }
        [[nodiscard]] Vec3 zyz() const { return Vec3(z, y, z); }
        [[nodiscard]] Vec3 zzx() const { return Vec3(z, z, x); }
        [[nodiscard]] Vec3 zzy() const { return Vec3(z, z, y); }
        [[nodiscard]] Vec3 zzz() const { return Vec3(z, z, z); }

        static Vec3 cross(const Vec3& a, const Vec3& b);
        static real dot(const Vec3& a, const Vec3& b);
        static Vec3 inverse(const Vec3& v);
        static Vec2 toVec2(const Vec3& v);
        static Vec4 toVec4(const Vec3& v);
        static Vec3 abs(const Vec3& v);
        static Vec3 lerp(const Vec3& a, const Vec3& b, real t);
        static Vec3 slerp(const Vec3& a, const Vec3& b, real t); // spherical linear interpolation
        static UVec3 nlerp(const Vec3& a, const Vec3& b, real t); // normalize linear interpolation (nlerp approximates slerp)
        static Vec3 project(const Vec3& v, const UVec3& onNormal);
        static Vec3 reject(const Vec3& v, const UVec3& onNormal);
        static UVec3 normalize(const Vec3& v);
        static Deg angleBetween(const UVec3& from, const UVec3& to); // already normalized angle between two vector
        static Deg angleBetween(const Vec3& from, const Vec3& to);
        static Vec3 reflect(const Vec3& a, const Vec3& b);
        static Vec3 negate(const Vec3& v);

        union {
            struct {
                real x;
                real y;
                real z;
            };
            real v[3];
        };
        static const Vec3 up, down, left, right, forward, backward, one, zero;

      private:
        static void swap(Vec3& first, Vec3& second);
    };
}  // namespace GLaDOS

#endif  //GLADOS_VEC3_H
