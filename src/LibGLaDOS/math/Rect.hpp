#ifndef GLADOS_RECT_HPP
#define GLADOS_RECT_HPP

#include "Math.h"
#include "Size.hpp"
#include "Point.hpp"

namespace GLaDOS {
    /*
                     (x+w,y+h)
        +-------------+
        |             |
        |             |
        |             |
        |             |
        +-------------+
      (x,y)
    */
    template <typename T>
    class Rect {
      public:
        Rect();
        Rect(const T& _x, const T& _y, const T& _w, const T& _h);
        Rect(const Rect<T>& other) = default;
        Rect(Rect<T>&& other) noexcept;
        Rect<T>& operator=(Rect<T> other);

        bool operator==(const Rect<T>& other) const;
        bool operator!=(const Rect<T>& other) const;
        Rect<T> operator+(const Rect<T>& other) const;
        Rect<T>& operator+=(const Rect<T>& other);
        Rect<T> operator-(const Rect<T>& other) const;
        Rect<T>& operator-=(const Rect<T>& other);
        Rect<T> operator*(const Rect<T>& other) const;
        Rect<T>& operator*=(const Rect<T>& other);
        Rect<T> operator/(const Rect<T>& other) const;
        Rect<T>& operator/=(const Rect<T>& other);

        Size<T> toSize() const;
        Rect& makeMerge(const Rect<T>& other);
        bool contains(const Point<T>& other) const;
        bool overlaps(const Rect<T>& other) const;
        T area() const;

        // anonymous union
        union {
            struct {
                T x, y;
                T w, h;
            };
            Point<T> origin;
            Size<T> size;
        };

        static Rect<T> fromPointSize(const Point<T>& _point, const Size<T>& _size); // make Rect from point and size
        static Rect<T> merge(const Rect<T>& a, const Rect<T>& b); // 합집합
        static Rect<T> intersection(const Rect<T>& a, const Rect<T>& b); // 교집합

        static const Rect<T> zero, one, unit;

      private:
        void swap(Rect<T>& first, Rect<T>& second);

        static bool inRange(T value, T min, T max);
        static bool inRange(T min0, T min1, T max0, T max1);
    };

    template <typename T>
    const Rect<T> Rect<T>::zero = Rect<T>{(T)0, (T)0, (T)0, (T)0};

    template <typename T>
    const Rect<T> Rect<T>::one = Rect<T>{(T)1, (T)1, (T)1, (T)1};

    template <typename T>
    const Rect<T> Rect<T>::unit = Rect<T>{(T)0, (T)0, (T)1, (T)1};

    template <typename T>
    Rect<T>::Rect() : x{(T)0}, y{(T)0}, w{(T)1}, h{(T)1} {
    }

    template <typename T>
    Rect<T>::Rect(const T& _x, const T& _y, const T& _w, const T& _h) : x{_x}, y{_y}, w{_w}, h{_h} {
    }

    template <typename T>
    Rect<T>::Rect(Rect<T>&& other) noexcept : Rect<T>{} {
        Rect<T>::swap(*this, other);
    }

    template <typename T>
    Rect<T>& Rect<T>::operator=(Rect<T> other) {
        // copy and swap idiom (effective c++ section 11)
        Rect<T>::swap(*this, other);
        return *this;
    }

    template <typename T>
    bool Rect<T>::operator==(const Rect<T>& other) const {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }

    template <typename T>
    bool Rect<T>::operator!=(const Rect<T>& other) const {
        return !(*this == other);
    }

    template <typename T>
    Rect<T> Rect<T>::operator+(const Rect<T>& other) const {
        // using op= (more effective c++ section 22)
        return Rect<T>(*this) += other;
    }

    template <typename T>
    Rect<T>& Rect<T>::operator+=(const Rect<T>& other) {
        x += other.x;
        y += other.y;
        w += other.w;
        h += other.h;
        return *this;
    }

    template <typename T>
    Rect<T> Rect<T>::operator-(const Rect<T>& other) const {
        return Rect<T>(*this) -= other;
    }

    template <typename T>
    Rect<T>& Rect<T>::operator-=(const Rect<T>& other) {
        x -= other.x;
        y -= other.y;
        w -= other.w;
        h -= other.h;
        return *this;
    }

    template <typename T>
    Rect<T> Rect<T>::operator*(const Rect<T>& other) const {
        return Rect<T>(*this) *= other;
    }

    template <typename T>
    Rect<T>& Rect<T>::operator*=(const Rect<T>& other) {
        x *= other.x;
        y *= other.y;
        w *= other.w;
        h *= other.h;
        return *this;
    }

    template <typename T>
    Rect<T> Rect<T>::operator/(const Rect<T>& other) const {
        return Rect<T>(*this) /= other;
    }

    template <typename T>
    Rect<T>& Rect<T>::operator/=(const Rect<T>& other) {
        x /= other.x;
        y /= other.y;
        w /= other.w;
        h /= other.h;
        return *this;
    }

    template <typename T>
    Size<T> Rect<T>::toSize() const {
        return Size<T>{w, h};
    }

    template <typename T>
    Rect<T>& Rect<T>::makeMerge(const Rect<T>& other) {
        return *this = Rect<T>::merge(*this, other);
    }

    template <typename T>
    bool Rect<T>::contains(const Point<T>& other) const {
        return Rect<T>::inRange(other.x, x, x + w) && Rect<T>::inRange(other.y, y, y + h);
    }

    template <typename T>
    bool Rect<T>::overlaps(const Rect<T>& other) const {
        return Rect<T>::inRange(x, other.x, x + w, other.x + other.w) && Rect<T>::inRange(y, other.y, y + h, other.y + other.h);
    }

    template <typename T>
    T Rect<T>::area() const {
        Size<T> size = toSize();
        return size.w * size.h;
    }

    template <typename T>
    Rect<T> Rect<T>::fromPointSize(const Point<T>& _point, const Size<T>& _size) {
        return Rect<T>{_point.x, _point.y, _size.w, _size.h};
    }

    template <typename T>
    Rect<T> Rect<T>::merge(const Rect<T>& a, const Rect<T>& b) {
        T newX = Math::min(Math::min(a.x, a.x + a.w), Math::min(b.x, b.x + b.w));
        T newY = Math::min(Math::min(a.y, a.y + a.h), Math::min(b.y, b.y + b.h));
        T newW = Math::max(Math::max(a.x, a.x + a.w), Math::max(b.x, b.x + b.w));
        T newH = Math::max(Math::max(a.y, a.y + a.h), Math::max(b.y, b.y + b.h));
        return Rect<T>{newX, newY, newW - newX, newH - newY};
    }

    template <typename T>
    Rect<T> Rect<T>::intersection(const Rect<T>& a, const Rect<T>& b) {
        if (!a.overlaps(b)) {
            return Rect<T>::zero;
        }
        T newX = Math::max(Math::min(a.x, a.x + a.w), Math::min(b.x, b.x + b.w));
        T newY = Math::max(Math::min(a.y, a.y + a.h), Math::min(b.y, b.y + b.h));
        T newW = Math::min(Math::max(a.x, a.x + a.w), Math::max(b.x, b.x + b.w));
        T newH = Math::min(Math::max(a.y, a.y + a.h), Math::max(b.y, b.y + b.h));
        return Rect<T>{newX, newY, newW - newX, newH - newY};
    }

    template <typename T>
    void Rect<T>::swap(Rect<T>& first, Rect<T>& second) {
        using std::swap;

        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.w, second.w);
        swap(first.h, second.h);
    }

    template <typename T>
    bool Rect<T>::inRange(T value, T min, T max) {
        return value >= Math::min(min, max) && value <= Math::max(min, max); // handle min, max flip over
    }

    template <typename T>
    bool Rect<T>::inRange(T min0, T min1, T max0, T max1) {
        return Math::max(min0, max0) >= Math::min(min1, max1) && Math::min(min0, max0) <= Math::max(min1, max1); // handle min, max flip over
    }
}  // namespace GLaDOS

#endif  //GLADOS_RECT_HPP
