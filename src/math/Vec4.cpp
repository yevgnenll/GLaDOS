#include "Vec4.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "Math.h"
#include "UVec4.h"
#include "Vec2.h"
#include "Vec3.h"

namespace GameEngine {
  Vec4::Vec4(real _x, real _y, real _z, real _w) : x{_x}, y{_y}, z{_z}, w{_w} {}

  Vec4::Vec4(const Vec2& other) : x{other.x}, y{other.y}, z{0.0}, w{1.0} {}

  Vec4::Vec4(const Vec3& other) : x{other.x}, y{other.y}, z{other.z}, w{1.0} {}

  Vec4::Vec4(const Vec2& other, real _z, real _w) : x{other.x}, y{other.y}, z{_z}, w{_w} {}

  Vec4::Vec4(const Vec3& other, real _w) : x{other.x}, y{other.y}, z{other.z}, w{_w} {}

  Vec4& Vec4::operator=(Vec4 other) {
    Vec4::swap(*this, other);
    return *this;
  }

  real& Vec4::operator[](unsigned int i) {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 3:
        return w;
      default:
        return x;
    }
  }

  const real& Vec4::operator[](unsigned int i) const {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 3:
        return w;
      default:
        return x;
    }
  }

  Vec4 Vec4::operator+(const Vec4& other) const {
    // using op= (more effective c++ section 22)
    return Vec4(*this) += other;
  }

  Vec4& Vec4::operator+=(const Vec4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
  }

  Vec4 Vec4::operator-(const Vec4& other) const {
    // using op= (more effective c++ section 22)
    return Vec4(*this) -= other;
  }

  Vec4& Vec4::operator-=(const Vec4& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
  }

  Vec4 Vec4::operator*(const Vec4& other) const {
    // using op= (more effective c++ section 22)
    return Vec4(*this) *= other;
  }

  Vec4& Vec4::operator*=(const Vec4& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;

    return *this;
  }

  Vec4 Vec4::operator/(const Vec4& other) const {
    // using op= (more effective c++ section 22)
    return Vec4(*this) /= other;
  }

  Vec4& Vec4::operator/=(const Vec4& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;

    return *this;
  }

  Vec4 Vec4::operator+(const real scalar) const { return Vec4(*this) += scalar; }

  Vec4& Vec4::operator+=(const real scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;

    return *this;
  }

  Vec4 Vec4::operator-(const real scalar) const { return Vec4(*this) -= scalar; }

  Vec4& Vec4::operator-=(const real scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;

    return *this;
  }

  Vec4 Vec4::operator*(const real scalar) const { return Vec4(*this) *= scalar; }

  Vec4& Vec4::operator*=(const real scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
  }

  Vec4 Vec4::operator/(const real scalar) const { return Vec4(*this) /= scalar; }

  Vec4& Vec4::operator/=(const real scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;
  }

  bool Vec4::operator==(const Vec4& other) const { return (Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z) && Math::equal(w, other.w)); }

  bool Vec4::operator!=(const Vec4& other) const { return !(*this == other); }

  Vec4 Vec4::operator-() const {
    return Vec4(-x, -y, -z, -w);
  }

  Vec4 Vec4::operator+() const {
    return Vec4(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z, w < 0 ? -w : w);
  }

  Vec4& Vec4::makeNegate() {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  UVec4 Vec4::makeNormalize() {
    real len = length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return UVec4{*this};
    }

    real inv = 1 / len;
    *this *= inv;
    return UVec4{*this};
  }

  real Vec4::length() const { return Math::sqrt(squaredLength()); }

  real Vec4::squaredLength() const { return x * x + y * y + z * z + w * w; }

  real Vec4::dot(const Vec4& a, const Vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

  Vec4 Vec4::inverse(const Vec4& other) { return Vec4(static_cast<real>(1.0 / other.x), static_cast<real>(1.0 / other.y), static_cast<real>(1.0 / other.z), static_cast<real>(1.0 / other.w)); }

  Vec2 Vec4::toVec2(const Vec4& other) { return Vec2(other); }

  Vec3 Vec4::toVec3(const Vec4& other) { return Vec3(other); }

  Vec4 Vec4::abs(const Vec4& other) { return Vec4(Math::abs(other.x), Math::abs(other.y), Math::abs(other.z), Math::abs(other.w)); }

  UVec4 Vec4::normalize(const Vec4& vector) {
    real len = vector.length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return UVec4{vector};
    }

    real inv = 1 / len;
    return UVec4{vector * inv};
  }

  Deg Vec4::angle(const UVec4& from, const UVec4& to) {
    return Math::toDegrees(Math::acos(Math::clamp(Vec4::dot(from, to), static_cast<real>(-1.0), static_cast<real>(1.0))));
  }

  void Vec4::swap(Vec4& first, Vec4& second) {
    using std::swap;

    swap(first.x, second.x);
    swap(first.y, second.y);
    swap(first.z, second.z);
    swap(first.w, second.w);
  }

  const UVec4 Vec4::up = UVec4{Vec4(0.0, 1.0, 0.0, 1.0)};
  const UVec4 Vec4::down = UVec4{Vec4(0.0, -1.0, 0.0, 1.0)};
  const UVec4 Vec4::left = UVec4{Vec4(-1.0, 0.0, 0.0, 1.0)};
  const UVec4 Vec4::right = UVec4{Vec4(1.0, 0.0, 0.0, 1.0)};
  const UVec4 Vec4::forward = UVec4{Vec4(0.0, 0.0, -1.0, 1.0)};
  const UVec4 Vec4::backward = UVec4{Vec4(0.0, 0.0, 1.0, 1.0)};
  const UVec4 Vec4::one = UVec4{Vec4(1.0, 1.0, 1.0, 1.0)};
  const UVec4 Vec4::zero = UVec4{Vec4(0.0, 0.0, 0.0, 0.0)};
}  // namespace GameEngine