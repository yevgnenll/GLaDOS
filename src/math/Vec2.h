#ifndef GAMEENGINE_VEC2_H
#define GAMEENGINE_VEC2_H

#include "utils/Enumeration.h"

namespace GameEngine {
  class UVec2;
  class Vec4;
  class Vec3;
  class Vec2 {
  public:
    Vec2() = default;
    explicit Vec2(real _x);
    Vec2(real _x, real _y);
    explicit Vec2(const Vec3& other);
    explicit Vec2(const Vec4& other);

    Vec2(const Vec2& other) = default;
    Vec2& operator=(Vec2 other);

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
    Vec2& makeNormalize();
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
    static Vec2 inverse(const Vec2& other);
    static Vec3 toVec3(const Vec2& other);
    static Vec4 toVec4(const Vec2& other);
    static Vec2 abs(const Vec2& other);
    static Vec2 lerp(const Vec2& a, const Vec2& b, real t);
    static Vec2 slerp(const Vec2& a, const Vec2& b, real t);
    static Vec2 nlerp(const Vec2& a, const Vec2& b, real t);
    static Vec2 normalize(const Vec2& vector);
    static Deg angle(const UVec2& from, const UVec2& to);

    real x{0.0}, y{0.0};
    static const UVec2 up, down, left, right, one, zero;

  private:
    static void swap(Vec2& first, Vec2& second);
  };
}  // namespace GameEngine

#endif  //GAMEENGINE_VEC2_H