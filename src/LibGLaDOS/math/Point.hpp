#ifndef GLADOS_POINT_H
#define GLADOS_POINT_H

#include "Math.h"

namespace GLaDOS {
    template <typename T>
    class Point {
      public:
        Point() : Point(0, 0) {}
        Point(T first, T second) : mFirst(first), mSecond(second) {}
        Point(const Point<T>& other) : mFirst(other.mFirst), mSecond(other.mSecond) {}

        T x() const { return mFirst; }
        T y() const { return mSecond; }

        T length() const {
            return static_cast<T>(Math::sqrt(mFirst * mFirst + mSecond * mSecond));
        }

        bool same() const {
            return mFirst == mSecond;
        }

        T distance(Point<T> v) const {
            return Point<T>(mFirst - v.mFirst, mSecond - v.mSecond).length();
        }

        void setX(T v) {
            mFirst = v;
        }

        void setY(T v) {
            mSecond = v;
        }

        void shiftX(T v) {
            mFirst += v;
        }

        void shiftY(T v) {
            mSecond += v;
        }

        void shift(T x, T y) {
            mFirst += x;
            mSecond += y;
        }

        Point<T>& operator=(const Point<T>& other) {
            mFirst = other.mFirst;
            mSecond = other.mSecond;
            return *this;
        }

        bool operator==(const Point<T>& v) const {
            return mFirst == v.mFirst && mSecond == v.mSecond;
        }

        bool operator!=(const Point<T>& v) const {
            return !(*this == v);
        }

        void operator+=(Point<T> v) {
            mFirst += v.mFirst;
            mSecond += v.mSecond;
        }

        void operator-=(Point<T> v) {
            mFirst -= v.mFirst;
            mSecond -= v.mSecond;
        }

        constexpr Point<T> operator-() const {
            return {-mFirst, -mSecond};
        }

        constexpr Point<T> operator+(T v) const {
            return {mFirst + v, mSecond + v};
        }

        constexpr Point<T> operator-(T v) const {
            return {mFirst - v, mSecond - v};
        }

        constexpr Point<T> operator*(T v) const {
            return {mFirst * v, mSecond * v};
        }

        constexpr Point<T> operator/(T v) const {
            return {mFirst / v, mSecond / v};
        }

        constexpr Point<T> operator+(Point<T> v) const {
            return {mFirst + v.mFirst, mSecond + v.mSecond};
        }

        constexpr Point<T> operator-(Point<T> v) const {
            return {mFirst - v.mFirst, mSecond - v.mSecond};
        }

        constexpr Point<T> operator*(Point<T> v) const {
            return {mFirst / v.mFirst, mSecond / v.mSecond};
        }

        constexpr Point<T> operator/(Point<T> v) const {
            return {
                mFirst / (v.mFirst == 0 ? 1 : v.mFirst),
                mSecond / (v.mSecond == 0 ? 1 : v.mSecond)};
        }

      private:
        T mFirst;
        T mSecond;
    };
}  // namespace GLaDOS

#endif  // GLADOS_POINT_H
