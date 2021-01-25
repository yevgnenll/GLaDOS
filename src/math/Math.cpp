#include "Math.h"

#include "Quat.h"

namespace GameEngine {
  real Math::clamp01(real value) { return value < real(0.0) ? real(0.0) : (value > real(1.0) ? real(1.0) : value); }

  real Math::lerp(real a, real b, real t) {
    return a + (b - a) * Math::clamp01(t);
  }

  real Math::lerpUnclamped(real a, real b, real t) {
    return a + (b - a) * t;
  }

  real Math::lerpAngle(real a, real b, real t) {
    real num = Math::deltaAngle(b - a, static_cast<real>(degrees));
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
    return f >= real(0.0) ? real(1.0) : real(-1.0);
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
    real deg = static_cast<real>(degrees);
    real num = Math::repeat(target - current, deg);
    if (num > (deg * real(0.5))) {
      num -= deg;
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
    real x = Math::max(static_cast<real>(0.0), Math::min(static_cast<real>(1.0), (value - min) / (max - min)));
    return x * x * (real(3.0) - real(2.0) * x);
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

  real Math::round(real a) { return a < real(0.0) ? std::ceil(a - real(0.5)) : std::floor(a + real(0.5)); }

  real Math::abs(real a) { return std::abs(a); }

  real Math::log(real a) { return std::log(a); }

  real Math::log10(real a) { return std::log10(a); }

  real Math::exp(real a) { return std::exp(a); }

  real Math::sqrt(real a) { return std::sqrt(a); }

  real Math::pow(real a, real exp) {
    return std::pow(a, exp);
  }

  real Math::dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  real Math::absDot(const Vec3& v1, const Vec3& v2) {
    return Math::abs(v1.x * v2.x) + Math::abs(v1.y * v2.y) + Math::abs(v1.z * v2.z);
  }

  Vec3 Math::pow(const Vec3& a, real exp) {
    return Vec3{Math::pow(a.x, exp), Math::pow(a.x, exp), Math::pow(a.x, exp)};
  }

  real Math::dot(const Vec2& v1, const Vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }

  real Math::absDot(const Vec2& v1, const Vec2& v2) {
    return Math::abs(v1.x * v2.x) + Math::abs(v1.y * v2.y);
  }

  Vec2 Math::pow(const Vec2& a, real exp) {
    return Vec2{Math::pow(a.x, exp), Math::pow(a.x, exp)};
  }

  Deg Math::pitch(const Quat& q) {
    return toDegrees(Math::atan2(real(2.0) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z));
  }

  Deg Math::yaw(const Quat& q) {
    return toDegrees(Math::asin(real(-2.0) * (q.x * q.z - q.w * q.y)));
  }

  Deg Math::roll(const Quat& q) {
    return toDegrees(Math::atan2(real(2.0) * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z));
  }
}  // namespace GameEngine