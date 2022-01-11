#ifndef GLADOS_VEC2_H
#define GLADOS_VEC2_H

#include "Angle.hpp"

namespace GLaDOS {
    class UVec2;
    class Vec4;
    class Vec3;
    class Vec2 {
      public:
        Vec2() = default;
        ~Vec2() = default;
        explicit Vec2(real _x);
        Vec2(real _x, real _y);
        explicit Vec2(const Vec3& other);
        explicit Vec2(const Vec4& other);
        Vec2(Vec2&& other) noexcept;
        Vec2(const Vec2& other) = default;
        Vec2& operator=(Vec2 other);  // copy and swap idiom

        real& operator[](unsigned int i);
        const real& operator[](unsigned int i) const;

        Vec2 operator+(const Vec2& other) const;
        Vec2& operator+=(const Vec2& other);

        Vec2 operator-(const Vec2& other) const;
        Vec2& operator-=(const Vec2& other);

        Vec2 operator*(const Vec2& other) const;
        Vec2& operator*=(const Vec2& other);

        Vec2 operator/(const Vec2& other) const;
        Vec2& operator/=(const Vec2& other);

        Vec2 operator+(real scalar) const;
        Vec2& operator+=(real scalar);

        Vec2 operator-(real scalar) const;
        Vec2& operator-=(real scalar);

        Vec2 operator*(real scalar) const;
        Vec2& operator*=(real scalar);

        Vec2 operator/(real scalar) const;
        Vec2& operator/=(real scalar);

        bool operator==(const Vec2& other) const;
        bool operator!=(const Vec2& other) const;

        Vec2 operator-() const;
        Vec2 operator+() const;
        Vec2& makeNegate();

        Vec2 perpendicular() const;
        UVec2 makeNormalize();
        real length() const;
        real squaredLength() const;
        real distance(const Vec2& other) const;
        real distanceSquare(const Vec2& other) const;

        [[nodiscard]] Vec2 xx() const { return Vec2(x, x); }
        [[nodiscard]] Vec2 xy() const { return Vec2(x, y); }
        [[nodiscard]] Vec2 yx() const { return Vec2(y, x); }
        [[nodiscard]] Vec2 yy() const { return Vec2(y, y); }

        static real cross(const Vec2& a, const Vec2& b);
        static real dot(const Vec2& a, const Vec2& b);
        static Vec2 inverse(const Vec2& v);
        static Vec3 toVec3(const Vec2& v);
        static Vec4 toVec4(const Vec2& v);
        static Vec2 abs(const Vec2& v);
        static Vec2 lerp(const Vec2& a, const Vec2& b, real t);
        static Vec2 slerp(const Vec2& a, const Vec2& b, real t); // spherical linear interpolation
        static UVec2 nlerp(const Vec2& a, const Vec2& b, real t); // normalize linear interpolation (nlerp approximates slerp)
        static UVec2 normalize(const Vec2& v);
        static Deg angleBetween(const UVec2& from, const UVec2& to);
        static Deg angleBetween(const Vec2& a, const Vec2& b);
        static Vec2 reflect(const Vec2& a, const Vec2& b);
        static Vec2 negate(const Vec2& v);

        real x{0.0}, y{0.0};
        static const Vec2 up, down, left, right, one, zero;

      private:
        static void swap(Vec2& first, Vec2& second);
    };
}  // namespace GLaDOS

#endif  //GLADOS_VEC2_H
