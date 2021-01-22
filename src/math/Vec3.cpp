#include "Vec3.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "Math.h"
#include "UVec3.h"
#include "Vec4.h"

namespace GameEngine {
  Vec3::Vec3(real _x) : x{_x}, y{0.0}, z{0.0} {}

  Vec3::Vec3(const real _x, const real _y) : x{_x}, y{_y}, z{0.0} {}

  Vec3::Vec3(const real _x, const real _y, const real _z) : x{_x}, y{_y}, z{_z} {}

  Vec3::Vec3(const Vec2& other) : x{other.x}, y{other.y}, z{0.0} {}

  Vec3::Vec3(const Vec4& other) : x{other.x}, y{other.y}, z{other.z} {}

  Vec3::Vec3(const Vec2& other, real _z) : x{other.x}, y{other.y}, z{_z} {}

  Vec3& Vec3::operator=(Vec3 other) {
    // copy and swap idiom (effective c++ section 11)
    Vec3::swap(*this, other);
    return *this;
  }

  real& Vec3::operator[](unsigned int i) {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        return x;
    }
  }

  const real& Vec3::operator[](unsigned int i) const {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        return x;
    }
  }

  Vec3 Vec3::operator+(const Vec3& other) const {
    // using op= (more effective c++ section 22)
    return Vec3(*this) += other;
  }

  Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vec3 Vec3::operator-(const Vec3& other) const { return Vec3(*this) -= other; }

  Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Vec3 Vec3::operator*(const Vec3& other) const { return Vec3(*this) *= other; }

  Vec3& Vec3::operator*=(const Vec3& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  Vec3 Vec3::operator/(const Vec3& other) const { return Vec3(*this) /= other; }

  Vec3& Vec3::operator/=(const Vec3& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  Vec3 Vec3::operator+(const real scalar) const { return Vec3(*this) += scalar; }

  Vec3& Vec3::operator+=(const real scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
  }

  Vec3 Vec3::operator-(const real scalar) const { return Vec3(*this) -= scalar; }

  Vec3& Vec3::operator-=(const real scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
  }

  Vec3 Vec3::operator*(const real scalar) const { return Vec3(*this) *= scalar; }

  Vec3& Vec3::operator*=(const real scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vec3 Vec3::operator/(const real scalar) const { return Vec3(*this) /= scalar; }

  Vec3& Vec3::operator/=(const real scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  bool Vec3::operator==(const Vec3& other) const { return (Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z)); }

  bool Vec3::operator!=(const Vec3& other) const { return !(*this == other); }

  Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }

  Vec3 Vec3::operator+() const { return Vec3(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z); }

  Vec3& Vec3::makeNegate() {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  Vec3 Vec3::perpendicular() const { return Vec3(-y, x, z); }

  UVec3 Vec3::makeNormalize() {
    real len = length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return UVec3{*this};
    }

    real inv = 1 / len;
    *this *= inv;
    return UVec3{*this};
  }

  real Vec3::length() const { return Math::sqrt(squaredLength()); }

  real Vec3::squaredLength() const { return x * x + y * y + z * z; }

  real Vec3::distance(const Vec3& other) const { return Math::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z)); }

  real Vec3::distanceSquare(const Vec3& other) const {
    Vec3 c = *this - other;
    return Vec3::dot(c, c);
  }

  Vec3 Vec3::cross(const Vec3& a, const Vec3& b) { return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

  real Vec3::dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

  Vec3 Vec3::inverse(const Vec3& other) { return Vec3(static_cast<real>(1.0 / other.x), static_cast<real>(1.0 / other.y), static_cast<real>(1.0 / other.z)); }

  Vec2 Vec3::toVec2(const Vec3& other) { return Vec2(other); }

  Vec4 Vec3::toVec4(const Vec3& other) { return Vec4(other); }

  Vec3 Vec3::abs(const Vec3& other) { return Vec3(Math::abs(other.x), Math::abs(other.y), Math::abs(other.z)); }

  Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, real t) { return (a + (b - a) * t); }

  Vec3 Vec3::slerp(const Vec3& a, const Vec3& b, real t) {
    // Dot product - the cosine of the angle between 2 vectors.
    real dot = Vec3::dot(a, b);
    Math::clamp(dot, static_cast<real>(-1.0), static_cast<real>(1.0));

    // Acos(dot) returns the angle between start and end,
    // And multiplying that by t returns the angle between
    // start and the final result.
    real theta = Math::acos(dot) * t;
    Vec3 rv = b - a * dot;
    rv.makeNormalize();

    return ((a * Math::cos(theta)) + (rv * Math::sin(theta)));
  }

  UVec3 Vec3::nlerp(const Vec3& a, const Vec3& b, real t) { return normalize(lerp(a, b, t)); }

  Vec3 Vec3::project(const Vec3& vector, const UVec3& onNormal) {
    real num = Vec3::dot(onNormal, onNormal);
    if (num < Math::realEpsilon) {
      return Vec3::zero;
    }
    return static_cast<Vec3>(onNormal) * Vec3::dot(vector, onNormal) / num;
  }

  Vec3 Vec3::projectOnPlane(const Vec3& vector, const UVec3& planeNormal) { return vector - Vec3::project(vector, planeNormal); }

  UVec3 Vec3::normalize(const Vec3& vector) {
    real len = vector.length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return UVec3{vector};
    }

    real inv = 1 / len;
    return UVec3{vector * inv};
  }

  Deg Vec3::angle(const UVec3& from, const UVec3& to) {
    return Math::toDegrees(Math::acos(Math::clamp(Vec3::dot(from, to), static_cast<real>(-1.0), static_cast<real>(1.0))));
  }

  void Vec3::swap(Vec3& first, Vec3& second) {
    using std::swap;

    swap(first.x, second.x);
    swap(first.y, second.y);
    swap(first.z, second.z);
  }

  const UVec3 Vec3::up = UVec3{Vec3(0.0, 1.0, 0.0)};
  const UVec3 Vec3::down = UVec3{Vec3(0.0, -1.0, 0.0)};
  const UVec3 Vec3::left = UVec3{Vec3(-1.0, 0.0, 0.0)};
  const UVec3 Vec3::right = UVec3{Vec3(1.0, 0.0, 0.0)};
  const UVec3 Vec3::forward = UVec3{Vec3(0.0, 0.0, -1.0)};
  const UVec3 Vec3::backward = UVec3{Vec3(0.0, 0.0, 1.0)};
  const UVec3 Vec3::one = UVec3{Vec3(1.0, 1.0, 1.0)};
  const UVec3 Vec3::zero = UVec3{Vec3(0.0, 0.0, 0.0)};
}  // namespace GameEngine
