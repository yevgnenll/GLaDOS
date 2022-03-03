#ifndef GLADOS_MATH_H
#define GLADOS_MATH_H

#undef max
#undef min

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>

#include "Angle.hpp"
#include "Vec2.h"
#include "Vec3.h"
#include "utils/Enumeration.h"
#include "FloatingPoint.h"

namespace GLaDOS {
    class Quat;
    class Math {
      public:
        Math() = delete;
        ~Math() = delete;

        template <typename T>
        static std::enable_if_t<!std::is_floating_point_v<T>, bool> equal(const T& a, const T& b);
        template <typename T>
        static std::enable_if_t<std::is_floating_point_v<T>, bool> equal(const T& a, const T& b);
        template <typename T>
        static std::enable_if_t<!std::is_floating_point_v<T>, bool> equalUlps(const T& a, const T& b, int ulpsTolerance);
        template <typename T>
        static std::enable_if_t<std::is_floating_point_v<T>, bool> equalUlps(const T& a, const T& b, int ulpsTolerance);
        template <typename T>
        static bool zero(const T& a);
        template <typename T>
        static T clamp(const T& x, const T& min, const T& max);
        static real clamp01(real value);
        template <typename T>
        static T lerp(T a, T b, real t);
        template <typename T>
        static T lerpUnclamped(T a, T b, real t);

        static real lerpAngle(real a, real b, real t);
        static real inverseLerp(real a, real b, real value);
        static Rad toRadians(Deg deg);
        static Vec3 toRadians(Vec3 degVec);
        static Deg toDegrees(Rad rad);
        static Vec3 toDegrees(Vec3 radVec);
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

        // easing functions: https://easings.net/#
        static real easeInSine(real a, real b, real t);
        static real easeOutSine(real a, real b, real t);
        static real easeInOutSine(real a, real b, real t);
        static real easeInQuad(real a, real b, real t);
        static real easeOutQuad(real a, real b, real t);
        static real easeInOutQuad(real a, real b, real t);
        static real easeInCubic(real a, real b, real t);
        static real easeOutCubic(real a, real b, real t);
        static real easeInOutCubic(real a, real b, real t);
        static real easeInQuat(real a, real b, real t);
        static real easeOutQuat(real a, real b, real t);
        static real easeInOutQuat(real a, real b, real t);
        static real easeInQuint(real a, real b, real t);
        static real easeOutQuint(real a, real b, real t);
        static real easeInOutQuint(real a, real b, real t);
        static real easeInExpo(real a, real b, real t);
        static real easeOutExpo(real a, real b, real t);
        static real easeInOutExpo(real a, real b, real t);
        static real easeInCirc(real a, real b, real t);
        static real easeOutCirc(real a, real b, real t);
        static real easeInOutCirc(real a, real b, real t);
        static real easeInBack(real a, real b, real t);
        static real easeOutBack(real a, real b, real t);
        static real easeInOutBack(real a, real b, real t);
        static real easeInElastic(real a, real b, real t);
        static real easeOutElastic(real a, real b, real t);
        static real easeInOutElastic(real a, real b, real t);
        static real easeInBounce(real a, real b, real t);
        static real easeOutBounce(real a, real b, real t);
        static real easeInOutBounce(real a, real b, real t);

        constexpr static uint64_t kilobytes(uint32_t kb);
        constexpr static uint64_t megabytes(uint32_t mb);
        constexpr static uint64_t gigabytes(uint32_t gb);

        static real sin(real angle);
        static real cos(real angle);
        static real tan(real angle);
        static real asin(real angle);
        static real acos(real angle);
        static real atan(real xOverY); // Quadrant 1,4 (-pi/2, pi/2)
        static real atan2(real y, real x); // Quadrant 1,2,3,4 (-pi, pi)
        static real csc(real angle);
        static real sec(real angle);
        static real cot(real angle);
        static real cosh(real angle);
        static real sinh(real angle);
        static real tanh(real angle);
        static real acosh(real angle);
        static real asinh(real angle);
        static real atanh(real angle);

        static real ceil(real a);
        static real floor(real a);
        static real round(real a);
        static real abs(real a);
        static real logBase(real a, real base);
        static real log(real a);
        static real log10(real a);
        static real log2(real a);
        static real exp(real a);
        static real sqrt(real a);
        static real rsqrt(real a);
        static real rsqrtEst(real a);
        static real pow(real a, real exp);
        static real mod(real a, real b);
        static bool isPowerOfTwo(uint32_t a);

        static real dot(const Vec3& v1, const Vec3& v2);
        static real absDot(const Vec3& v1, const Vec3& v2);
        static Vec3 pow(const Vec3& a, real exp);

        static real dot(const Vec2& v1, const Vec2& v2);
        static real absDot(const Vec2& v1, const Vec2& v2);
        static Vec2 pow(const Vec2& a, real exp);

        template <typename T>
        static constexpr auto max(const T& a, const T& b);
        template <typename T, typename... Ts>
        static constexpr auto max(const T& a, const T& b, const Ts&... ts);
        template <typename T>
        static constexpr auto min(const T& a, const T& b);
        template <typename T, typename... Ts>
        static constexpr auto min(const T& a, const T& b, const Ts&... ts);

        static Deg pitch(const Quat& q);
        static Deg yaw(const Quat& q);
        static Deg roll(const Quat& q);
        static constexpr bool isDigit(char x);
        static char* dtoa(char* s, double n);
        static bool atod(const char* s, const char* s_end, double* result);

        static constexpr real pi = real(3.14159265358979);  // std::atan(1) * 4
        static constexpr int degrees = 360;
        static constexpr real deg2Rad = Math::pi / real(180);
        static constexpr real rad2Deg = real(180) / Math::pi;
        static constexpr real realEpsilon = std::numeric_limits<real>::epsilon();
        static constexpr real realInfinity = std::numeric_limits<real>::infinity();
        static constexpr uint16_t shortMax = std::numeric_limits<uint16_t>::max();
        static constexpr uint16_t maxNumberStringSize = 32;
    };

    template <typename T>
    std::enable_if_t<!std::is_floating_point_v<T>, bool> Math::equal(const T& a, const T& b) {
        return a == b;
    }

    template <typename T>
    std::enable_if_t<std::is_floating_point_v<T>, bool> Math::equal(const T& a, const T& b) {
        return Real<T>(a) == Real<T>(b);
    }

    template <typename T>
    std::enable_if_t<!std::is_floating_point_v<T>, bool> Math::equalUlps(const T& a, const T& b, int ulpsTolerance) {
        return a == b;
    }

    template <typename T>
    std::enable_if_t<std::is_floating_point_v<T>, bool> Math::equalUlps(const T& a, const T& b, int ulpsTolerance) {
        static T tolerance = std::numeric_limits<T>::epsilon();
        return compareTo(Real<T>(a), Real<T>(b), ulpsTolerance, tolerance) == 0;
    }

    template <typename T>
    bool Math::zero(const T& a) {
        return equal(a, static_cast<const T>(0));
    }

    template <typename T>
    T Math::clamp(const T& x, const T& min, const T& max) {
        return x < min ? min : (x > max ? max : x);
    }

    template <typename T>
    T Math::lerp(T a, T b, real t) {
        return a + (b - a) * Math::clamp(t, real(0), real(1));
    }

    template <typename T>
    T Math::lerpUnclamped(T a, T b, real t) {
        return a + (b - a) * t;
    }

    template <typename T>
    constexpr auto Math::max(const T& a, const T& b) {
        return (a < b) ? b : a;
    }

    template <typename T, typename... Ts>
    constexpr auto Math::max(const T& a, const T& b, const Ts&... ts) {
        const auto m = a < b ? b : a;
        if constexpr (sizeof...(ts) > 0) {
            return max(m, ts...);
        }
        return m;
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
}  // namespace GLaDOS

#endif
