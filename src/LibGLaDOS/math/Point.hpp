#ifndef GLADOS_POINT_H
#define GLADOS_POINT_H

#include "Math.h"

namespace GLaDOS {
    template <typename T>
    class Point {
      public:
        Point() : Point(0, 0) {}
        Point(T first, T second) : _x(first), _y(second) {}
        Point(const Point<T>& other) : _x(other._x), _y(other._y) {}

        T x() const { return _x; }
        T y() const { return _y; }

        T& operator[](unsigned int i) {
            switch (i) {
                case 0:
                    return _x;
                case 1:
                    return _y;
                default:
                    return _x;
            }
        }

        const T& operator[](unsigned int i) const {
            switch (i) {
                case 0:
                    return _x;
                case 1:
                    return _y;
                default:
                    return _x;
            }
        }

        T length() const {
            return static_cast<T>(Math::sqrt(_x * _x + _y * _y));
        }

        bool same() const {
            return _x == _y;
        }

        T distance(Point<T> v) const {
            return Point<T>(_x - v._x, _y - v._y).length();
        }

        void setX(T v) {
            _x = v;
        }

        void setY(T v) {
            _y = v;
        }

        void shiftX(T v) {
            _x += v;
        }

        void shiftY(T v) {
            _y += v;
        }

        void shift(T x, T y) {
            _x += x;
            _y += y;
        }

        Point<T>& operator=(const Point<T>& other) {
            _x = other._x;
            _y = other._y;
            return *this;
        }

        bool operator==(const Point<T>& v) const {
            return _x == v._x && _y == v._y;
        }

        bool operator!=(const Point<T>& v) const {
            return !(*this == v);
        }

        void operator+=(Point<T> v) {
            _x += v._x;
            _y += v._y;
        }

        void operator-=(Point<T> v) {
            _x -= v._x;
            _y -= v._y;
        }

        constexpr Point<T> operator-() const {
            return {-_x, -_y};
        }

        constexpr Point<T> operator+(T v) const {
            return {_x + v, _y + v};
        }

        constexpr Point<T> operator-(T v) const {
            return {_x - v, _y - v};
        }

        constexpr Point<T> operator*(T v) const {
            return {_x * v, _y * v};
        }

        constexpr Point<T> operator/(T v) const {
            return {_x / v, _y / v};
        }

        constexpr Point<T> operator+(Point<T> v) const {
            return {_x + v._x, _y + v._y};
        }

        constexpr Point<T> operator-(Point<T> v) const {
            return {_x - v._x, _y - v._y};
        }

        constexpr Point<T> operator*(Point<T> v) const {
            return {_x / v._x, _y / v._y};
        }

        constexpr Point<T> operator/(Point<T> v) const {
            return {
                _x / (v._x == 0 ? 1 : v._x),
                _y / (v._y == 0 ? 1 : v._y)};
        }

      private:
        T _x;
        T _y;
    };
}  // namespace GLaDOS

#endif  // GLADOS_POINT_H
