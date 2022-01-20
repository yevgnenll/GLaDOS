#ifndef GLADOS_POINT_H
#define GLADOS_POINT_H

#include "Math.h"
#include "Vec2.h"

namespace GLaDOS {
    template <typename T>
    class Point {
      public:
        Point();
        Point(T _x, T _y);
        Point(const Point<T>& other) = default;
        Point(Point<T>&& other) noexcept;
        Point<T>& operator=(Point<T> other);

        T& operator[](unsigned int i);
        const T& operator[](unsigned int i) const;

        T length() const;
        T distance(const Point<T>& p) const;

        bool operator==(const Point<T>& p) const;
        bool operator!=(const Point<T>& p) const;

        Point<T> operator-() const;
        Point<T> operator+(T scalar) const;
        Point<T> operator-(T scalar) const;
        Point<T> operator*(T scalar) const;
        Point<T> operator/(T scalar) const;

        Point<T> operator+(const Vec2& vector) const; // point + vector = point
        Point<T> operator-(const Vec2& vector) const; // point - vector = point
        Point<T>& operator+=(const Vec2& vector);
        Point<T>& operator-=(const Vec2& vector);
        // point + point = undefined
        Vec2 operator-(const Point<T>& point) const; // point - point = vector

        union {
            struct {
                T x;
                T y;
            };
            T v[2];
        };

      private:
        void swap(Point<T>& first, Point<T>& second);
    };

    template <typename T>
    Point<T>::Point() : x{(T)0}, y{(T)0} {
    }

    template <typename T>
    Point<T>::Point(T _x, T _y) : x{_x}, y{_y} {
    }

    template <typename T>
    Point<T>::Point(Point<T>&& other) noexcept {
        Point<T>::swap(*this, other);
    }

    template <typename T>
    Point<T>& Point<T>::operator=(Point<T> other) {
        // copy and swap idiom (effective c++ section 11)
        Point<T>::swap(*this, other);
        return *this;
    }

    template <typename T>
    T& Point<T>::operator[](unsigned int i) {
        return v[i];
    }

    template <typename T>
    const T& Point<T>::operator[](unsigned int i) const {
        return v[i];
    }

    template <typename T>
    T Point<T>::length() const {
        return static_cast<T>(Math::sqrt(x * x + y * y));
    }

    template <typename T>
    T Point<T>::distance(const Point<T>& p) const {
        return Point<T>(x - p.x, y - p.y).length();
    }

    template <typename T>
    bool Point<T>::operator==(const Point<T>& p) const {
        return x == p.x && y == p.y;
    }

    template <typename T>
    bool Point<T>::operator!=(const Point<T>& p) const {
        return !(*this == p);
    }

    template <typename T>
    Point<T> Point<T>::operator-() const {
        return Point<T>{-x, -y};
    }

    template <typename T>
    Point<T> Point<T>::operator+(T scalar) const {
        return Point<T>{x + scalar, y + scalar};
    }

    template <typename T>
    Point<T> Point<T>::operator-(T scalar) const {
        return Point<T>{x - scalar, y - scalar};
    }

    template <typename T>
    Point<T> Point<T>::operator*(T scalar) const {
        return Point<T>{x * scalar, y * scalar};
    }

    template <typename T>
    Point<T> Point<T>::operator/(T scalar) const {
        return Point<T>{x / scalar, y / scalar};
    }

    template <typename T>
    Point<T> Point<T>::operator+(const Vec2& vector) const {
        return Point<T>(x + vector.x, y + vector.y);
    }

    template <typename T>
    Point<T> Point<T>::operator-(const Vec2& vector) const {
        return Point<T>(x - vector.x, y - vector.y);
    }

    template <typename T>
    Point<T>& Point<T>::operator+=(const Vec2& vector) {
        x += vector.x;
        y += vector.y;
        return *this;
    }

    template <typename T>
    Point<T>& Point<T>::operator-=(const Vec2& vector) {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }

    template <typename T>
    Vec2 Point<T>::operator-(const Point<T>& point) const {
        return Vec2(x - point.x, y - point.y);
    }

    template <typename T>
    void Point<T>::swap(Point<T>& first, Point<T>& second) {
        using std::swap;

        swap(first.x, second.x);
        swap(first.y, second.y);
    }
}  // namespace GLaDOS

#endif  // GLADOS_POINT_H
