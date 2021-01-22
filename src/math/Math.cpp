#include "Math.h"

namespace GameEngine {
  real Math::clamp01(real value) { return value < 0.F ? 0.F : (value > 1.F ? 1.F : value); }

  real Math::lerp(real a, real b, real t) {
    return a + (b - a) * Math::clamp01(t);
  }

  real Math::lerpUnclamped(real a, real b, real t) {
    return a + (b - a) * t;
  }

  real Math::lerpAngle(real a, real b, real t) {
    real num = Math::deltaAngle(b - a, 360.F);
    return a + num * Math::clamp01(t);
  }

  real Math::inverseLerp(real a, real b, real value) {
    if (Math::equal(a, b)) {
      return 0.F;
    }
    return Math::clamp01((value - a) / (b - a));
  }

  Rad Math::toRadians(real deg) { return Rad{deg * deg2Rad}; }

  Deg Math::toDegrees(real rad) { return Deg{rad * rad2Deg}; }

  real Math::sign(real f) {
    return f >= 0.F ? 1.F : -1.F;
  }

  real Math::repeat(real t, real length) {
    /*
      it is never larger than length and never smaller than 0.
      Math::Reapeat(-100, 360) -> 260
      Math::Reapeat(370, 360) -> 10
      Math::Reapeat(250, 360) -> 250
    */
    return t - Math::floor(t / length) * length;
  }

  real Math::deltaAngle(real current, real target) {
    float num = Math::repeat(target - current, 360.F);
    if (num > 180.F) {
      num -= 360.F;
    }
    return num;
  }

  real Math::pingPong(real t, real len) {
    real L = 2 * len;
    real T = std::fmod(t, L);

    if (0 <= T && T < len) {
      return T;
    }

    return (L - T);
  }

  real Math::perlinNoise(real x) {
    // TODO
    return 0.f;
  }

  real Math::perlinNoise(real x, real y) {
    // TODO
    return 0.f;
  }

  real Math::perlinNoise(real x, real y, real z) {
    // TODO
    return 0.f;
  }

  real Math::smoothStep(real min, real max, real value) {
    float x = Math::max(0.F, Math::min(1.F, (value - min) / (max - min)));
    return x * x * (3 - 2 * x);
  }

  real Math::moveTowards(real current, real target, real maxDelta) {
    if (Math::abs(target - current) <= maxDelta) {
      return target;
    }
    return current + Math::sign(target - current) * maxDelta;
  }

  real Math::moveTowardsAngle(real current, real target, real maxDelta) {
    target = current + Math::deltaAngle(current, target);
    return Math::moveTowards(current, target, maxDelta);
  }

  real Math::easeIn() {
    // TODO
    return 0.f;
  }

  real Math::sin(real angle) { return std::sin(angle); }

  real Math::cos(real angle) { return std::cos(angle); }

  real Math::tan(real angle) { return std::tan(angle); }

  real Math::asin(real angle) { return std::asin(angle); }

  real Math::acos(real angle) { return std::acos(angle); }

  real Math::atan(real angle) { return std::atan(angle); }

  real Math::atan2(real y, real x) { return ::atan2(y, x); }

  real Math::ceil(real a) { return std::ceil(a); }

  real Math::floor(real a) { return std::floor(a); }

  real Math::round(real a) { return a < 0.F ? std::ceil(a - 0.5F) : std::floor(a + 0.5F); }

  real Math::abs(real a) { return std::abs(a); }

  real Math::log(real a) { return std::log(a); }

  real Math::log10(real a) { return std::log10(a); }

  real Math::exp(real a) { return std::exp(a); }

  real Math::sqrt(real a) { return std::sqrt(a); }

  real Math::pow(real a, real exp) {
    return std::pow(a, exp);
  }

  float Math::dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  float Math::absDot(const Vec3& v1, const Vec3& v2) {
    return Math::abs(v1.x * v2.x) + Math::abs(v1.y * v2.y) + Math::abs(v1.z * v2.z);
  }

  Vec3 Math::pow(const Vec3& a, float exp) {
    return Vec3{Math::pow(a.x, exp), Math::pow(a.x, exp), Math::pow(a.x, exp)};
  }

  float Math::dot(const Vec2& v1, const Vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }

  float Math::absDot(const Vec2& v1, const Vec2& v2) {
    return Math::abs(v1.x * v2.x) + Math::abs(v1.y * v2.y);
  }

  Vec2 Math::pow(const Vec2& a, float exp) {
    return Vec2{Math::pow(a.x, exp), Math::pow(a.x, exp)};
  }
}  // namespace GameEngine