#include "Vec2.h"

#include "Math.h"
#include "UVec2.h"
#include "Vec4.h"

namespace GLaDOS {
    Vec2::Vec2() : x{0.0}, y{0.0} {}

    Vec2::Vec2(real _x) : x{_x}, y{0.0} {}

    Vec2::Vec2(real _x, real _y) : x{_x}, y{_y} {}

    Vec2::Vec2(const Vec3& other) : x{other.x}, y{other.y} {}

    Vec2::Vec2(const Vec4& other) : x{other.x}, y{other.y} {}

    Vec2::Vec2(Vec2&& other) noexcept : Vec2{} {
        Vec2::swap(*this, other);
    }

    Vec2& Vec2::operator=(Vec2 other) {
        // copy and swap idiom (effective c++ section 11)
        Vec2::swap(*this, other);
        return *this;
    }

    real& Vec2::operator[](unsigned int i) {
        return v[i];
    }

    const real& Vec2::operator[](unsigned int i) const {
        return v[i];
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

    Vec2 Vec2::operator-() const { return Vec2{-x, -y}; }

    Vec2 Vec2::operator+() const { return Vec2{x < 0 ? -x : x, y < 0 ? -y : y}; }

    Vec2& Vec2::makeNegate() {
        x = -x;
        y = -y;
        return *this;
    }

    Vec2 Vec2::perpendicular() const { return Vec2{-y, x}; }

    UVec2 Vec2::makeNormalize() {
        real len = length();
        if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
            return UVec2{*this};
        }

        real inv = 1 / len;
        *this *= inv;
        return UVec2{*this};
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

    Vec2 Vec2::inverse(const Vec2& v) { return Vec2{real(1.0) / v.x, real(1.0) / v.y}; }

    Vec3 Vec2::toVec3(const Vec2& v) { return Vec3{v}; }

    Vec4 Vec2::toVec4(const Vec2& v) { return Vec4{v}; }

    Vec2 Vec2::abs(const Vec2& v) { return Vec2{Math::abs(v.x), Math::abs(v.y)}; }

    Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, real t) { return Vec2{a + (b - a) * t}; }

    Vec2 Vec2::slerp(const Vec2& a, const Vec2& b, real t) {
        // Dot product - the cosine of the angle between 2 vectors.
        real dot = Vec2::dot(a, b);
        dot = Math::clamp(dot, static_cast<real>(-1.0), static_cast<real>(1.0));

        // acos(dot) returns the angle between start and end,
        // And multiplying that by t returns the angle between
        // start and the final result.
        real theta = Math::acos(dot) * t;
        Vec2 rv = b - a * dot;
        rv.makeNormalize();

        return Vec2{(a * Math::cos(theta)) + (rv * Math::sin(theta))};
    }

    UVec2 Vec2::nlerp(const Vec2& a, const Vec2& b, real t) { return Vec2::normalize(Vec2::lerp(a, b, t)); }

    UVec2 Vec2::normalize(const Vec2& v) {
        real len = v.length();
        if (Math::zero(len) || Math::equal(len, static_cast<real>(1.0))) {
            return UVec2{v};
        }

        real inv = 1 / len;
        return UVec2{v * inv};
    }

    Deg Vec2::angleBetween(const UVec2& from, const UVec2& to) {
        return Math::toDegrees(Rad{Math::acos(Math::clamp(Vec2::dot(from, to), static_cast<real>(-1.0), static_cast<real>(1.0)))});
    }

    Deg Vec2::angleBetween(const Vec2& from, const Vec2& to) {
        real lengthInv = 1 / (from.length() * to.length());
        real dot = Vec2::dot(from, to);
        return Math::toDegrees(Rad{Math::acos(dot * lengthInv)});
    }

    Vec2 Vec2::reflect(const Vec2& a, const Vec2& b) {
        real len = b.length();
        if (len < Math::realEpsilon) {
            return Vec2::zero;
        }
        real scale = Vec2::dot(a, b) / len;
        Vec2 proj2 = b * (scale * 2);
        return a - proj2;
    }

    Vec2 Vec2::negate(const Vec2& v) {
        return Vec2{-v.x, -v.y};
    }

    void Vec2::swap(Vec2& first, Vec2& second) {
        using std::swap;

        swap(first.x, second.x);
        swap(first.y, second.y);
    }

    const Vec2 Vec2::up = Vec2{0.0, 1.0};
    const Vec2 Vec2::down = Vec2{0.0, -1.0};
    const Vec2 Vec2::left = Vec2{-1.0, 0.0};
    const Vec2 Vec2::right = Vec2{1.0, 0.0};
    const Vec2 Vec2::one = Vec2{1.0, 1.0};
    const Vec2 Vec2::zero = Vec2{0.0, 0.0};
}  // namespace GLaDOS