#include "Vec2.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "Math.h"
#include "UVec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace GameEngine {
  Vec2::Vec2(real _x) : x{_x}, y{0.0} {}

  Vec2::Vec2(real _x, real _y) : x{_x}, y{_y} {}

  Vec2::Vec2(const Vec3& other) : x{other.x}, y{other.y} {}

  Vec2::Vec2(const Vec4& other) : x{other.x}, y{other.y} {}

  Vec2& Vec2::operator=(Vec2 other) {
    // copy and swap idiom (effective c++ section 11)
    Vec2::swap(*this, other);
    return *this;
  }

  real& Vec2::operator[](unsigned int i) {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      default:
        return x;
    }
  }

  const real& Vec2::operator[](unsigned int i) const {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      default:
        return x;
    }
  }

  Vec2 Vec2::operator+(const Vec2& other) const {
    // using op= (more effective c++ section 22)
    return Vec2(*this) += other;
  }

  Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vec2 Vec2::operator-(const Vec2& other) const { return Vec2(*this) -= other; }

  Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vec2 Vec2::operator*(const Vec2& other) const { return Vec2(*this) *= other; }

  Vec2& Vec2::operator*=(const Vec2& other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  Vec2 Vec2::operator/(const Vec2& other) const { return Vec2(*this) /= other; }

  Vec2& Vec2::operator/=(const Vec2& other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  Vec2 Vec2::operator+(const real scalar) const { return Vec2(*this) += scalar; }

  Vec2& Vec2::operator+=(const real scalar) {
    x += scalar;
    y += scalar;
    return *this;
  }

  Vec2 Vec2::operator-(const real scalar) const { return Vec2(*this) -= scalar; }

  Vec2& Vec2::operator-=(const real scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
  }

  Vec2 Vec2::operator*(const real scalar) const { return Vec2(*this) *= scalar; }

  Vec2& Vec2::operator*=(const real scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vec2 Vec2::operator/(const real scalar) const { return Vec2(*this) /= scalar; }

  Vec2& Vec2::operator/=(const real scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  bool Vec2::operator==(const Vec2& other) const { return (Math::equal(x, other.x) && Math::equal(y, other.y)); }

  bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }

  Vec2 Vec2::operator-() const { return Vec2(-x, -y); }

  Vec2 Vec2::operator+() const { return Vec2(x < 0 ? -x : x, y < 0 ? -y : y); }

  Vec2& Vec2::makeNegate() {
    x = -x;
    y = -y;
    return *this;
  }

  Vec2 Vec2::perpendicular() const { return Vec2(-y, x); }

  Vec2& Vec2::makeNormalize() {
    real len = Math::sqrt(x * x + y * y);
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return *this;
    }

    real inv = 1 / len;

    x = x * inv;
    y = y * inv;

    return *this;
  }

  real Vec2::length() const { return Math::sqrt(squaredLength()); }

  real Vec2::squaredLength() const {
    return x * x + y * y;
  }

  real Vec2::distance(const Vec2& other) const { return Math::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)); }

  real Vec2::distanceSquare(const Vec2& other) const {
    Vec2 c = *this - other;
    return Vec2::dot(c, c);
  }

  real Vec2::cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }

  real Vec2::dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

  Vec2 Vec2::inverse(const Vec2& other) {
    return Vec2(static_cast<real>(1.0 / other.x), static_cast<real>(1.0 / other.y));
  }

  Vec3 Vec2::toVec3(const Vec2& other) { return Vec3(other); }

  Vec4 Vec2::toVec4(const Vec2& other) { return Vec4(other); }

  Vec2 Vec2::abs(const Vec2& other) { return Vec2(Math::abs(other.x), Math::abs(other.y)); }

  Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, real t) { return (a + (b - a) * t); }

  Vec2 Vec2::slerp(const Vec2& a, const Vec2& b, real t) {
    // Dot product - the cosine of the angle between 2 vectors.
    real dot = Vec2::dot(a, b);
    // Math::clamp(dot, -1.0f, 1.0f);

    // Acos(dot) returns the angle between start and end,
    // And multiplying that by t returns the angle between
    // start and the final result.
    real theta = Math::acos(dot) * t;
    Vec2 rv = b - a * dot;
    rv.makeNormalize();

    return ((a * Math::cos(theta)) + (rv * Math::sin(theta)));
  }

  Vec2 Vec2::nlerp(const Vec2& a, const Vec2& b, real t) {
    Vec2 lerp = Vec2::lerp(a, b, t);
    return lerp.makeNormalize();
  }

  Vec2 Vec2::normalize(const Vec2& vector) {
    real len = vector.length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return vector;
    }

    real inv = 1 / len;
    return Vec2{vector * inv};
  }

  Deg Vec2::angle(const UVec2& from, const UVec2& to) {
    return Math::toDegrees(Math::acos(Math::clamp(Vec2::dot(from, to), static_cast<real>(-1.0), static_cast<real>(1.0))));
  }

  void Vec2::swap(Vec2& first, Vec2& second) {
    using std::swap;

    swap(first.x, second.x);
    swap(first.y, second.y);
  }

  const UVec2 Vec2::up = UVec2{Vec2(0.0, 1.0)};
  const UVec2 Vec2::down = UVec2{Vec2(0.0, -1.0)};
  const UVec2 Vec2::left = UVec2{Vec2(-1.0, 0.0)};
  const UVec2 Vec2::right = UVec2{Vec2(1.0, 0.0)};
  const UVec2 Vec2::one = UVec2{Vec2(1.0, 1.0)};
  const UVec2 Vec2::zero = UVec2{Vec2(0.0, 0.0)};
}  // namespace GameEngine