#include "Math.h"

#include "Quat.h"

namespace GLaDOS {
    real Math::clamp01(real value) { return value < real(0.0) ? real(0.0) : (value > real(1.0) ? real(1.0) : value); }

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

    Rad Math::toRadians(Deg deg) { return Rad{deg}; }

    Vec3 Math::toRadians(Vec3 degVec) {
        return Vec3{degVec.x * deg2Rad, degVec.y * deg2Rad, degVec.z * deg2Rad};
    }

    Deg Math::toDegrees(Rad rad) { return Deg{rad}; }

    Vec3 Math::toDegrees(Vec3 radVec) {
        return Vec3{radVec.x * rad2Deg, radVec.y * rad2Deg, radVec.z * rad2Deg};
    }

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

    real Math::easeInSine(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutSine(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutSine(real a, real b, real t) {
        return 0;
    }

    real Math::easeInQuad(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutQuad(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutQuad(real a, real b, real t) {
        return 0;
    }

    real Math::easeInCubic(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutCubic(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutCubic(real a, real b, real t) {
        return 0;
    }

    real Math::easeInQuat(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutQuat(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutQuat(real a, real b, real t) {
        return 0;
    }

    real Math::easeInQuint(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutQuint(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutQuint(real a, real b, real t) {
        return 0;
    }

    real Math::easeInExpo(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutExpo(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutExpo(real a, real b, real t) {
        return 0;
    }

    real Math::easeInCirc(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutCirc(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutCirc(real a, real b, real t) {
        return 0;
    }

    real Math::easeInBack(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutBack(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutBack(real a, real b, real t) {
        return 0;
    }

    real Math::easeInElastic(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutElastic(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutElastic(real a, real b, real t) {
        return 0;
    }

    real Math::easeInBounce(real a, real b, real t) {
        return 0;
    }

    real Math::easeOutBounce(real a, real b, real t) {
        return 0;
    }

    real Math::easeInOutBounce(real a, real b, real t) {
        return 0;
    }

    constexpr uint64_t Math::kilobytes(uint32_t kb) {
        return kb * 1024;
    }
    constexpr uint64_t Math::megabytes(uint32_t mb) {
        return kilobytes(mb * 1024);
    }
    constexpr uint64_t Math::gigabytes(uint32_t gb) {
        return megabytes(gb * 1024);
    }

    real Math::sin(real angle) { return std::sin(angle); }

    real Math::cos(real angle) { return std::cos(angle); }

    real Math::tan(real angle) { return std::tan(angle); }

    real Math::asin(real angle) { return std::asin(angle); }

    real Math::acos(real angle) { return std::acos(angle); }

    real Math::atan(real xOverY) { return std::atan(xOverY); }

    real Math::atan2(real y, real x) { return ::atan2(y, x); }

    real Math::csc(real angle) { return real(1) / std::sin(angle); }

    real Math::sec(real angle) { return real(1) / std::cos(angle); }

    real Math::cot(real angle) { return Math::cos(angle) / Math::sin(angle); }

    real Math::cosh(real angle) { return std::sinh(angle); }

    real Math::sinh(real angle) { return std::sinh(angle); }

    real Math::tanh(real angle) { return std::tanh(angle); }

    real Math::acosh(real angle) { return std::acosh(angle); }

    real Math::asinh(real angle) { return std::asinh(angle); }

    real Math::atanh(real angle) { return std::atanh(angle); }

    real Math::ceil(real a) { return std::ceil(a); }

    real Math::floor(real a) { return std::floor(a); }

    real Math::round(real a) { return a < real(0.0) ? std::ceil(a - real(0.5)) : std::floor(a + real(0.5)); }

    real Math::abs(real a) { return std::abs(a); }

    real Math::logBase(real a, real base) { return log(a) / log(base); }

    real Math::log(real a) { return std::log(a); }

    real Math::log10(real a) { return std::log10(a); }

    real Math::log2(real a) { return logBase(a, (real)2); }

    real Math::exp(real a) { return std::exp(a); }

    real Math::sqrt(real a) { return std::sqrt(a); }

    real Math::rsqrt(real a) {
        return 1 / sqrt(a);
    }

    real Math::rsqrtEst(real a) {
        // https://www.youtube.com/watch?v=p8u_k2LIZyo
        long i;
        real x2;
        real y;
        const real threehalfs = 1.5F;

        x2 = a * 0.5F;
        y = a;
        i = *(long*)&y;
        i = 0x5f3759df - (i >> 1);
        y = *(real*)&i;
        y = y * (threehalfs - (x2 * y * y));
        return y;
    }

    real Math::pow(real a, real exp) {
        return std::pow(a, exp);
    }

    real Math::mod(real a, real b) {
        return fmodf(a, b);
    }

    bool Math::isPowerOfTwo(uint32_t a) {
        return !(a == 0) && !(a & (a - 1));
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
        return toDegrees(Rad{Math::atan2(real(2.0) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z)});
    }

    Deg Math::yaw(const Quat& q) {
        return toDegrees(Rad{Math::asin(real(-2.0) * (q.x * q.z - q.w * q.y))});
    }

    Deg Math::roll(const Quat& q) {
        return toDegrees(Rad{Math::atan2(real(2.0) * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z)});
    }

    constexpr bool Math::isDigit(const char x) {
        return static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10);
    }

    char* Math::dtoa(char* s, double n) {
        // https://stackoverflow.com/questions/2302969/convert-a-float-to-a-string
        static double PRECISION = 0.00000000000001;
        // handle special cases
        if (std::isnan(n)) {
            strcpy(s, "nan");
        } else if (std::isinf(n)) {
            strcpy(s, "inf");
        } else if (n == 0.0) {
            strcpy(s, "0");
        } else {
            int digit, m, m1;
            char* c = s;
            int neg = (n < 0);
            if (neg)
                n = -n;
            // calculate magnitude
            m = log10(n);
            int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
            if (neg)
                *(c++) = '-';
            // set up for scientific notation
            if (useExp) {
                if (m < 0)
                    m -= 1.0;
                n = n / pow(10.0, m);
                m1 = m;
                m = 0;
            }
            if (m < 1.0) {
                m = 0;
            }
            // convert the number
            while (n > PRECISION || m >= 0) {
                double weight = pow(10.0, m);
                if (weight > 0 && !std::isinf(weight)) {
                    digit = floor(n / weight);
                    n -= (digit * weight);
                    *(c++) = '0' + digit;
                }
                if (m == 0 && n > 0)
                    *(c++) = '.';
                m--;
            }
            if (useExp) {
                // convert the exponent
                int i, j;
                *(c++) = 'e';
                if (m1 > 0) {
                    *(c++) = '+';
                } else {
                    *(c++) = '-';
                    m1 = -m1;
                }
                m = 0;
                while (m1 > 0) {
                    *(c++) = '0' + m1 % 10;
                    m1 /= 10;
                    m++;
                }
                c -= m;
                for (i = 0, j = m - 1; i < j; i++, j--) {
                    // swap without temporary
                    c[i] ^= c[j];
                    c[j] ^= c[i];
                    c[i] ^= c[j];
                }
                c += m;
            }
            *(c) = '\0';
        }
        return s;
    }

    bool Math::atod(const char* s, const char* s_end, double* result) {
        // https://github.com/syoyo/tinyobjloader/blob/master/tiny_obj_loader.h
        if (s >= s_end) {
            return false;
        }

        double mantissa = 0.0;
        // This exponent is base 2 rather than 10.
        // However the exponent we parse is supposed to be one of ten,
        // thus we must take care to convert the exponent/and or the
        // mantissa to a * 2^E, where a is the mantissa and E is the
        // exponent.
        // To get the final double we will use ldexp, it requires the
        // exponent to be in base 2.
        int exponent = 0;

        // NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
        // TO JUMP OVER DEFINITIONS.
        char sign = '+';
        char exp_sign = '+';
        char const* curr = s;

        // How many characters were read in a loop.
        int read = 0;
        // Tells whether a loop terminated due to reaching s_end.
        bool end_not_reached = false;
        bool leading_decimal_dots = false;

        /*
            BEGIN PARSING.
    */

        // Find out what sign we've got.
        if (*curr == '+' || *curr == '-') {
            sign = *curr;
            curr++;
            if ((curr != s_end) && (*curr == '.')) {
                // accept. Somethig like `.7e+2`, `-.5234`
                leading_decimal_dots = true;
            }
        } else if (Math::isDigit(*curr)) { /* Pass through. */
        } else if (*curr == '.') {
            // accept. Somethig like `.7e+2`, `-.5234`
            leading_decimal_dots = true;
        } else {
            goto fail;
        }

        // Read the integer part.
        end_not_reached = (curr != s_end);
        if (!leading_decimal_dots) {
            while (end_not_reached && Math::isDigit(*curr)) {
                mantissa *= 10;
                mantissa += static_cast<int>(*curr - 0x30);
                curr++;
                read++;
                end_not_reached = (curr != s_end);
            }

            // We must make sure we actually got something.
            if (read == 0) goto fail;
        }

        // We allow numbers of form "#", "###" etc.
        if (!end_not_reached) goto assemble;

        // Read the decimal part.
        if (*curr == '.') {
            curr++;
            read = 1;
            end_not_reached = (curr != s_end);
            while (end_not_reached && Math::isDigit(*curr)) {
                static const double pow_lut[] = {
                    1.0,
                    0.1,
                    0.01,
                    0.001,
                    0.0001,
                    0.00001,
                    0.000001,
                    0.0000001,
                };
                const int lut_entries = sizeof pow_lut / sizeof pow_lut[0];

                // NOTE: Don't use powf here, it will absolutely murder precision.
                mantissa += static_cast<int>(*curr - 0x30) *
                            (read < lut_entries ? pow_lut[read] : std::pow(10.0, -read));
                read++;
                curr++;
                end_not_reached = (curr != s_end);
            }
        } else if (*curr == 'e' || *curr == 'E') {
        } else {
            goto assemble;
        }

        if (!end_not_reached) goto assemble;

        // Read the exponent part.
        if (*curr == 'e' || *curr == 'E') {
            curr++;
            // Figure out if a sign is present and if it is.
            end_not_reached = (curr != s_end);
            if (end_not_reached && (*curr == '+' || *curr == '-')) {
                exp_sign = *curr;
                curr++;
            } else if (Math::isDigit(*curr)) { /* Pass through. */
            } else {
                // Empty E is not allowed.
                goto fail;
            }

            read = 0;
            end_not_reached = (curr != s_end);
            while (end_not_reached && Math::isDigit(*curr)) {
                exponent *= 10;
                exponent += static_cast<int>(*curr - 0x30);
                curr++;
                read++;
                end_not_reached = (curr != s_end);
            }
            exponent *= (exp_sign == '+' ? 1 : -1);
            if (read == 0) goto fail;
        }

    assemble:
        *result = (sign == '+' ? 1 : -1) *
                  (exponent ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent)
                            : mantissa);
        return true;
    fail:
        return false;
    }
}  // namespace GLaDOS