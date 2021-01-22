#ifndef GAMEENGINE_MATH_H
#define GAMEENGINE_MATH_H

#include <cmath>
#include <limits>
#include <type_traits>

#include "Angle.hpp"
#include "Vec2.h"
#include "Vec3.h"
#include "utils/Enumeration.h"

namespace GameEngine {
  class Math {
  public:
    Math() = delete;

    template <typename T>
    static std::enable_if_t<not std::is_floating_point_v<T>, bool> equal(const T& a, const T& b);
    template <typename T>
    static std::enable_if_t<std::is_floating_point_v<T>, bool> equal(const T& a, const T& b);
    template <typename T>
    static bool zero(const T& a);
    template <typename T>
    static real clamp(const T& x, const T& min, const T& max);
    static real clamp01(real value);
    static real lerp(real a, real b, real t);
    static real lerpUnclamped(real a, real b, real t);
    static real lerpAngle(real a, real b, real t);
    static real inverseLerp(real a, real b, real value);
    static Rad toRadians(real deg);
    static Deg toDegrees(real rad);
    static real sign(real f);
    static real repeat(real t, real length);
    static real deltaAngle(real current, real target);
    static real pingPong(real t, real len);
    static real perlinNoise(real x);
    static real perlinNoise(real x, real y);
    static real perlinNoise(real x, real y, real z);
    static real smoothStep(real min, real max, real value);
    static real moveTowards(real current, real target, real maxDelta);
    static real moveTowardsAngle(real current, real target, real maxDelta);
    static real easeIn();

    static real sin(real angle);
    static real cos(real angle);
    static real tan(real angle);
    static real asin(real angle);
    static real acos(real angle);
    static real atan(real angle);
    static real atan2(real y, real x);

    static real ceil(real a);
    static real floor(real a);
    static real round(real a);
    static real abs(real a);
    static real log(real a);
    static real log10(real a);
    static real exp(real a);
    static real sqrt(real a);
    static real pow(real a, real exp);

    static float dot(const Vec3& v1, const Vec3& v2);
    static float absDot(const Vec3& v1, const Vec3& v2);
    static Vec3 pow(const Vec3& a, float exp);

    static float dot(const Vec2& v1, const Vec2& v2);
    static float absDot(const Vec2& v1, const Vec2& v2);
    static Vec2 pow(const Vec2& a, float exp);

    template <typename T>
    static constexpr auto max(const T& a, const T& b);
    template <typename T>
    static constexpr auto min(const T& a, const T& b);
    template <typename T, typename... Ts>
    static constexpr auto min(const T& a, const T& b, const Ts&... ts);
    template <typename T, typename... Ts>
    static constexpr auto max(const T& a, const T& b, const Ts&... ts);

    static constexpr real pi = 3.14159265358979F;  // std::atan(1) * 4
    static constexpr int degrees = 360;
    static constexpr real deg2Rad = Math::pi / 180.F;
    static constexpr real rad2Deg = 180.F / Math::pi;
    static constexpr real realEpsilon = std::numeric_limits<real>::epsilon();
    static constexpr real realInfinity = std::numeric_limits<real>::infinity();
  };

  template <typename T>
  std::enable_if_t<not std::is_floating_point_v<T>, bool> Math::equal(const T& a, const T& b) {
    return a == b;
  }

  template <typename T>
  std::enable_if_t<std::is_floating_point_v<T>, bool> Math::equal(const T& a, const T& b) {
    //Threshold denominator so we don't divide by zero
    static constexpr auto tolerance = std::numeric_limits<T>::epsilon();
    static constexpr auto threshold = std::numeric_limits<T>::min();  //A very small nonzero number!
    auto min = Math::min(std::abs(a), std::abs(b));
    if (std::abs(min) == 0.0) {
      return std::abs(a - b) < tolerance;
    }
    return (std::abs(a - b) / Math::max(threshold, min)) < tolerance;
  }

  template <typename T>
  bool Math::zero(const T& a) {
    return equal(a, static_cast<const T>(0));
  }

  template <typename T>
  float Math::clamp(const T& x, const T& min, const T& max) {
    return x < min ? min : (x > max ? max : x);
  }

  template <typename T>
  constexpr auto Math::max(const T& a, const T& b) {
    return (a < b) ? b : a;
  }

  template <typename T>
  constexpr auto Math::min(const T& a, const T& b) {
    return (a < b) ? a : b;
  }

  template <typename T, typename... Ts>
  constexpr auto Math::min(const T& a, const T& b, const Ts&... ts) {
    const auto m = a < b ? a : b;
    if constexpr (sizeof...(ts) > 0) {
      return min(m, ts...);
    }
    return m;
  }

  template <typename T, typename... Ts>
  constexpr auto Math::max(const T& a, const T& b, const Ts&... ts) {
    const auto m = a < b ? b : a;
    if constexpr (sizeof...(ts) > 0) {
      return max(m, ts...);
    }
    return m;
  }
}  // namespace GameEngine

#endif