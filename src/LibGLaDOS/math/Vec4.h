#ifndef GLADOS_VEC4_H
#define GLADOS_VEC4_H

#include "Angle.hpp"
#include "Vec2.h"
#include "Vec3.h"

namespace GLaDOS {
    template <typename T>
    class Mat4;
    class UVec4;
    class Vec4 {
      public:
        Vec4();
        ~Vec4() = default;
        explicit Vec4(real _xyzw);
        Vec4(real _x, real _y, real _z, real _w);
        explicit Vec4(const Vec2& other);
        explicit Vec4(const Vec3& other);
        Vec4(const Vec2& other, real _z, real _w);
        Vec4(const Vec3& other, real _w);
        Vec4(Vec4&& other) noexcept;
        Vec4(const Vec4& other) = default;
        Vec4& operator=(Vec4 other);  // copy and swap idiom

        real& operator[](unsigned int i);
        const real& operator[](unsigned int i) const;

        Vec4 operator+(const Vec4& other) const;
        Vec4& operator+=(const Vec4& other);

        Vec4 operator-(const Vec4& other) const;
        Vec4& operator-=(const Vec4& other);

        Vec4 operator*(const Mat4<real>& m) const;
        Vec4 operator*=(const Mat4<real>& m);

        Vec4 operator+(real scalar) const;
        Vec4& operator+=(real scalar);

        Vec4 operator-(real scalar) const;
        Vec4& operator-=(real scalar);

        Vec4 operator*(real scalar) const;
        Vec4& operator*=(real scalar);

        Vec4 operator/(real scalar) const;
        Vec4& operator/=(real scalar);

        bool operator==(const Vec4& other) const;
        bool operator!=(const Vec4& other) const;

        Vec4 operator-() const;
        Vec4 operator+() const;
        Vec4& makeNegate();

        UVec4 makeNormalize();
        real length() const;
        real squaredLength() const;

        [[nodiscard]] Vec2 xx() const { return Vec2(x, x); }
        [[nodiscard]] Vec2 xy() const { return Vec2(x, y); }
        [[nodiscard]] Vec2 xz() const { return Vec2(x, z); }
        [[nodiscard]] Vec2 xw() const { return Vec2(x, w); }
        [[nodiscard]] Vec2 yx() const { return Vec2(y, x); }
        [[nodiscard]] Vec2 yy() const { return Vec2(y, y); }
        [[nodiscard]] Vec2 yz() const { return Vec2(y, z); }
        [[nodiscard]] Vec2 yw() const { return Vec2(y, w); }
        [[nodiscard]] Vec2 zx() const { return Vec2(z, x); }
        [[nodiscard]] Vec2 zy() const { return Vec2(z, y); }
        [[nodiscard]] Vec2 zz() const { return Vec2(z, z); }
        [[nodiscard]] Vec2 zw() const { return Vec2(z, w); }

        [[nodiscard]] Vec3 xxx() const { return Vec3(x, x, x); }
        [[nodiscard]] Vec3 xxy() const { return Vec3(x, x, y); }
        [[nodiscard]] Vec3 xxz() const { return Vec3(x, x, z); }
        [[nodiscard]] Vec3 xxw() const { return Vec3(x, x, w); }
        [[nodiscard]] Vec3 xyx() const { return Vec3(x, y, x); }
        [[nodiscard]] Vec3 xyy() const { return Vec3(x, y, y); }
        [[nodiscard]] Vec3 xyz() const { return Vec3(x, y, z); }
        [[nodiscard]] Vec3 xyw() const { return Vec3(x, y, w); }
        [[nodiscard]] Vec3 xzx() const { return Vec3(x, z, x); }
        [[nodiscard]] Vec3 xzy() const { return Vec3(x, z, y); }
        [[nodiscard]] Vec3 xzz() const { return Vec3(x, z, z); }
        [[nodiscard]] Vec3 xzw() const { return Vec3(x, z, w); }
        [[nodiscard]] Vec3 xwx() const { return Vec3(x, w, x); }
        [[nodiscard]] Vec3 xwy() const { return Vec3(x, w, y); }
        [[nodiscard]] Vec3 xwz() const { return Vec3(x, w, z); }
        [[nodiscard]] Vec3 xww() const { return Vec3(x, w, w); }

        [[nodiscard]] Vec3 yxx() const { return Vec3(y, x, x); }
        [[nodiscard]] Vec3 yxy() const { return Vec3(y, x, y); }
        [[nodiscard]] Vec3 yxz() const { return Vec3(y, x, z); }
        [[nodiscard]] Vec3 yxw() const { return Vec3(y, x, w); }
        [[nodiscard]] Vec3 yyx() const { return Vec3(y, y, x); }
        [[nodiscard]] Vec3 yyy() const { return Vec3(y, y, y); }
        [[nodiscard]] Vec3 yyz() const { return Vec3(y, y, z); }
        [[nodiscard]] Vec3 yyw() const { return Vec3(y, y, w); }
        [[nodiscard]] Vec3 yzx() const { return Vec3(y, z, x); }
        [[nodiscard]] Vec3 yzy() const { return Vec3(y, z, y); }
        [[nodiscard]] Vec3 yzz() const { return Vec3(y, z, z); }
        [[nodiscard]] Vec3 yzw() const { return Vec3(y, z, w); }
        [[nodiscard]] Vec3 ywx() const { return Vec3(y, w, x); }
        [[nodiscard]] Vec3 ywy() const { return Vec3(y, w, y); }
        [[nodiscard]] Vec3 ywz() const { return Vec3(y, w, z); }
        [[nodiscard]] Vec3 yww() const { return Vec3(y, w, w); }

        [[nodiscard]] Vec3 zxx() const { return Vec3(z, x, x); }
        [[nodiscard]] Vec3 zxy() const { return Vec3(z, x, y); }
        [[nodiscard]] Vec3 zxz() const { return Vec3(z, x, z); }
        [[nodiscard]] Vec3 zxw() const { return Vec3(z, x, w); }
        [[nodiscard]] Vec3 zyx() const { return Vec3(z, y, x); }
        [[nodiscard]] Vec3 zyy() const { return Vec3(z, y, y); }
        [[nodiscard]] Vec3 zyz() const { return Vec3(z, y, z); }
        [[nodiscard]] Vec3 zyw() const { return Vec3(z, y, w); }
        [[nodiscard]] Vec3 zzx() const { return Vec3(z, z, x); }
        [[nodiscard]] Vec3 zzy() const { return Vec3(z, z, y); }
        [[nodiscard]] Vec3 zzz() const { return Vec3(z, z, z); }
        [[nodiscard]] Vec3 zzw() const { return Vec3(z, z, w); }
        [[nodiscard]] Vec3 zwx() const { return Vec3(z, w, x); }
        [[nodiscard]] Vec3 zwy() const { return Vec3(z, w, y); }
        [[nodiscard]] Vec3 zwz() const { return Vec3(z, w, z); }
        [[nodiscard]] Vec3 zww() const { return Vec3(z, w, w); }

        [[nodiscard]] Vec3 wxx() const { return Vec3(w, x, x); }
        [[nodiscard]] Vec3 wxy() const { return Vec3(w, x, y); }
        [[nodiscard]] Vec3 wxz() const { return Vec3(w, x, z); }
        [[nodiscard]] Vec3 wxw() const { return Vec3(w, x, w); }
        [[nodiscard]] Vec3 wyx() const { return Vec3(w, y, x); }
        [[nodiscard]] Vec3 wyy() const { return Vec3(w, y, y); }
        [[nodiscard]] Vec3 wyz() const { return Vec3(w, y, z); }
        [[nodiscard]] Vec3 wyw() const { return Vec3(w, y, w); }
        [[nodiscard]] Vec3 wzx() const { return Vec3(w, z, x); }
        [[nodiscard]] Vec3 wzy() const { return Vec3(w, z, y); }
        [[nodiscard]] Vec3 wzz() const { return Vec3(w, z, z); }
        [[nodiscard]] Vec3 wzw() const { return Vec3(w, z, w); }
        [[nodiscard]] Vec3 wwx() const { return Vec3(w, w, x); }
        [[nodiscard]] Vec3 wwy() const { return Vec3(w, w, y); }
        [[nodiscard]] Vec3 wwz() const { return Vec3(w, w, z); }
        [[nodiscard]] Vec3 www() const { return Vec3(w, w, w); }

        static real dot(const Vec4& a, const Vec4& b);
        static Vec4 inverse(const Vec4& v);
        static Vec2 toVec2(const Vec4& v);
        static Vec3 toVec3(const Vec4& v);
        static Vec4 abs(const Vec4& v);
        static Vec4 sqrt(const Vec4& v);
        static Vec4 rsqrt(const Vec4& v);
        static UVec4 normalize(const Vec4& v);
        static Deg angleBetween(const UVec4& from, const UVec4& to);
        static Deg angleBetween(const Vec4& from, const Vec4& to);

        union {
            struct {
                real x;
                real y;
                real z;
                real w;
            };
            real v[4];
        };
        static const Vec4 up, down, left, right, forward, backward, one, zero;

      private:
        static void swap(Vec4& first, Vec4& second);
    };
}  // namespace GLaDOS

#endif  //GLADOS_VEC4_H
