#include "Vec4.h"

#include "Math.h"
#include "UVec4.h"

namespace GLaDOS {
  Vec4::Vec4(real _x, real _y, real _z, real _w) : x{_x}, y{_y}, z{_z}, w{_w} {}

  Vec4::Vec4(const Vec2& other) : x{other.x}, y{other.y}, z{0.0}, w{1.0} {}

  Vec4::Vec4(const Vec3& other) : x{other.x}, y{other.y}, z{other.z}, w{1.0} {}

  Vec4::Vec4(const Vec2& other, real _z, real _w) : x{other.x}, y{other.y}, z{_z}, w{_w} {}

  Vec4::Vec4(const Vec3& other, real _w) : x{other.x}, y{other.y}, z{other.z}, w{_w} {}

  Vec4::Vec4(Vec4&& other) noexcept : Vec4{} {
    Vec4::swap(*this, other);
  }

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
    return Vec4{-x, -y, -z, -w};
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

  Vec4 Vec4::inverse(const Vec4& v) { return Vec4{real(1.0) / v.x, real(1.0) / v.y, real(1.0) / v.z, real(1.0) / v.w}; }

  Vec2 Vec4::toVec2(const Vec4& v) { return Vec2{v}; }

  Vec3 Vec4::toVec3(const Vec4& v) { return Vec3{v}; }

  Vec4 Vec4::abs(const Vec4& v) { return Vec4{Math::abs(v.x), Math::abs(v.y), Math::abs(v.z), Math::abs(v.w)}; }

  Vec4 Vec4::sqrt(const Vec4& v) { return Vec4{Math::sqrt(v.x), Math::sqrt(v.y), Math::sqrt(v.z), Math::sqrt(v.w)}; }

  Vec4 Vec4::rsqrt(const Vec4& v) { return Vec4{Math::rsqrt(v.x), Math::rsqrt(v.y), Math::rsqrt(v.z), Math::rsqrt(v.w)}; }

  Vec4 Vec4::min(const Vec4& a, const Vec4& b) {
    return Vec4{Math::min(a.x, b.x), Math::min(a.y, b.y), Math::min(a.z, b.z), Math::min(a.w, b.w)};
  }

  Vec4 Vec4::max(const Vec4& a, const Vec4& b) {
    return Vec4{Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z), Math::max(a.w, b.w)};
  }

  UVec4 Vec4::normalize(const Vec4& v) {
    real len = v.length();
    if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
      return UVec4{v};
    }

    real inv = 1 / len;
    return UVec4{v * inv};
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

  const Vec4 Vec4::up = Vec4{0.0, 1.0, 0.0, 1.0};
  const Vec4 Vec4::down = Vec4{0.0, -1.0, 0.0, 1.0};
  const Vec4 Vec4::left = Vec4{-1.0, 0.0, 0.0, 1.0};
  const Vec4 Vec4::right = Vec4{1.0, 0.0, 0.0, 1.0};
  const Vec4 Vec4::forward = Vec4{0.0, 0.0, -1.0, 1.0};
  const Vec4 Vec4::backward = Vec4{0.0, 0.0, 1.0, 1.0};
  const Vec4 Vec4::one = Vec4{1.0, 1.0, 1.0, 1.0};
  const Vec4 Vec4::zero = Vec4{0.0, 0.0, 0.0, 0.0};
}  // namespace GLaDOS