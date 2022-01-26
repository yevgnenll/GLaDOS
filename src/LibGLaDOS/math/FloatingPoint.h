#ifndef GLADOS_FLOATINGPOINT_H
#define GLADOS_FLOATINGPOINT_H

#include <cmath>
#include <limits>

namespace GLaDOS {
    template <typename T>
    union Real {};

    template <typename T>
    int compareTo(const Real<T>& a, const Real<T>& b, int ulpsTolerance, T absTolerance) {
        float diff = a.f - b.f;

        // check absolute difference
        if (std::abs(diff) <= absTolerance) {
            return 0;
        }

        // different signs means they do not match
        if (a.negative() != b.negative()) {
            // check for equality to make sure +0 == -0
            if (a.f == b.f) {
                return 0;
            }
            return (diff > 0) ? 1 : -1;
        }

        // find the difference in ULPS
        int32_t ulpsDiff = std::abs(a.i - b.i);
        if (ulpsDiff <= ulpsTolerance) {
            return 0;
        }

        return (diff > 0) ? 1 : -1;
    }

    template <>
    union Real<float> {
        typedef int32_t IntType;
        typedef float RealType;

        Real(RealType num = static_cast<RealType>(0)) : f{num} {}

        bool negative() const { return i < 0; }
        IntType mantissa() const { return i & ((1 << 23) - 1); }
        IntType exponent() const { return (i >> 23) & 0xFF; }

        Real<float>& operator=(const Real<float>& other) {
            f = other.f;
            return *this;
        }

        bool operator==(const Real<float>& other) const {
            static RealType tolerance = std::numeric_limits<RealType>::epsilon();
            return compareTo(*this, other, 4, tolerance) == 0;
        }

        IntType i;
        RealType f;
    };

    template <>
    union Real<double> {
        typedef int64_t IntType;
        typedef double RealType;

        Real(RealType num = static_cast<RealType>(0)) : f{num} {}

        bool negative() const { return i < 0; }
        IntType mantissa() const { return i & ((IntType(1) << 52) - 1); }
        IntType exponent() const { return (i >> 52) & ((IntType(1) << 11) - 1); }

        Real<double>& operator=(const Real<double>& other) {
            f = other.f;
            return *this;
        }

        bool operator==(const Real<double>& other) const {
            static RealType tolerance = std::numeric_limits<RealType>::epsilon();
            return compareTo(*this, other, 4, tolerance) == 0;
        }

        IntType i;
        RealType f;
    };
}

#endif  // GLADOS_FLOATINGPOINT_H
