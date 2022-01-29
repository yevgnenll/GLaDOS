#ifndef GLADOS_HERMITESPLINE_HPP
#define GLADOS_HERMITESPLINE_HPP

#include "math/Math.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    template <typename T>
    class HermiteSpline {
      public:
        HermiteSpline(const T& point1, const T& slope1, const T& point2, const T& slope2);
        T interpolate(real t);

      private:
        T mPoint1;
        T mSlope1;
        T mPoint2;
        T mSlope2;
    };

    template <typename T>
    HermiteSpline<T>::HermiteSpline(const T& point1, const T& slope1, const T& point2, const T& slope2)
        : mPoint1{point1}, mSlope1{slope1}, mPoint2{point2}, mSlope2{slope2} {
    }

    template <typename T>
    T HermiteSpline<T>::interpolate(real t) {
        return mPoint1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
               mSlope1 * (t * ((1.0f - t) * (1.0f - t))) +
               mPoint2 * ((t * t) * (3.0f - 2.0f * t)) +
               mSlope2 * ((t * t) * (t - 1.0f));
    }
}

#endif  // GLADOS_HERMITESPLINE_HPP
