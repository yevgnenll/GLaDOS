#ifndef GLADOS_BEZIERSPLINE_HPP
#define GLADOS_BEZIERSPLINE_HPP

#include "math/Math.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    template <typename T>
    class BezierSpline {
      public:
        BezierSpline(const T& point1, const T& control1, const T& point2, const T& control2);
        T interpolate(real t);

      private:
        T mPoint1;
        T mControl1;
        T mPoint2;
        T mControl2;
    };

    template <typename T>
    BezierSpline<T>::BezierSpline(const T& point1, const T& control1, const T& point2, const T& control2)
        : mPoint1{point1}, mControl1{control1}, mPoint2{point2}, mControl2{control2} {
    }

    template <typename T>
    T BezierSpline<T>::interpolate(real t) {
        T A = Math::lerp(mPoint1, mControl1, t);
        T B = Math::lerp(mControl2, mPoint2, t);
        T C = Math::lerp(mControl1, mControl2, t);
        T D = Math::lerp(A, C, t);
        T E = Math::lerp(C, B, t);
        return Math::lerp(D, E, t);
    }
}

#endif  // GLADOS_BEZIERSPLINE_HPP
