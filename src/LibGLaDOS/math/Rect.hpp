#ifndef GLADOS_RECT_HPP
#define GLADOS_RECT_HPP

#include "Math.h"
#include "Size.hpp"

namespace GLaDOS {
    /*
                     (w,h)
        +-------------+
        |             |
        |             |
        |             |
        |             |
        +-------------+
      (x,y)
    */
    class Vec2;
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
        bool intersect(const Rect<T>& other) const;
        bool contains(const Vec2& value) const;
        bool contains(const Rect<T>& value) const;
        Rect<T>& setRect(const T& _x, const T& _y, const T& _w, const T& _h);
        T area() const;
        void makeExpand(T to);

        // anonymous union
        union {
            struct {
                T x, y;
                T w, h;
            };
            Size<T> origin;
            Size<T> size;
        };

        static Rect<T> expand(Rect<T>& rect, T to);
        static Rect<T> fromPoints(const Vec2& p1, const Vec2& p2);
        static Rect<T> merge(const Rect<T>& a, const Rect<T>& b);  // 합집합
        static Rect<T> intersection(const Rect<T>& a, const Rect<T>& b);  // 교집합

        static const Rect<T> zero, one;

      private:
        void swap(Rect<T>& first, Rect<T>& second);
    };

    template <typename T>
    const Rect<T> Rect<T>::zero = Rect<T>{};

    template <typename T>
    const Rect<T> Rect<T>::one = Rect<T>{(T)1, (T)1, (T)1, (T)1};

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
        return Size<T>{w - x, h - y};
    }

    template <typename T>
    Rect<T>& Rect<T>::makeMerge(const Rect<T>& other) {
        x = Math::min(x, other.x);
        y = Math::max(y, other.y);
        w = Math::min(w, other.w);
        h = Math::max(h, other.h);
        return *this;
    }

    template <typename T>
    bool Rect<T>::intersect(const Rect<T>& other) const {
        Rect rect;
        rect.x = Math::max(x, other.x);
        rect.y = Math::max(y, other.y);
        rect.w = Math::min(w, other.w);
        rect.h = Math::min(h, other.h);
        return !static_cast<bool>(rect.x > rect.w || rect.y > rect.h);
    }

    template <typename T>
    bool Rect<T>::contains(const Vec2& value) const {
        return !static_cast<bool>(x > value.x || w < value.x || y > value.y || h < value.y);
    }

    template <typename T>
    bool Rect<T>::contains(const Rect<T>& value) const {
        Size<T> thisSize = toSize();
        Size<T> otherSize = value.toSize();
        return x <= value.x && otherSize.x <= thisSize.x && y <= value.y && otherSize.y <= thisSize.y;
    }

    template <typename T>
    Rect<T>& Rect<T>::setRect(const T& _x, const T& _y, const T& _w, const T& _h) {
        return *this = Rect<T>{_x, _y, _w, _h};
    }

    template <typename T>
    T Rect<T>::area() const {
        Size<T> size = toSize();
        return size.x * size.y;
    }

    template <typename T>
    void Rect<T>::makeExpand(T to) {
        x -= to * 0.5;
        y -= to * 0.5;
        w += to;
        h += to;
    }

    template <typename T>
    Rect<T> Rect<T>::expand(Rect<T>& rect, T to) {
        rect.x -= to * 0.5;
        rect.y -= to * 0.5;
        rect.w += to;
        rect.h += to;
    }

    template <typename T>
    Rect<T> Rect<T>::fromPoints(const Vec2& p1, const Vec2& p2) {
        //    const Vec2 upperLeft = Vec2::min(p1, p2);
        //    const Vec2 rightBottom = Vec2::max(p1, p2);
        //    return Rect<T>(upperLeft, Math::Max(rightBottom - upperLeft, Vector2::Zero));
        // TODO
    }

    template <typename T>
    Rect<T> Rect<T>::merge(const Rect<T>& a, const Rect<T>& b) {
        T newX = Math::min(a.x, b.x);
        T newY = Math::min(a.y, b.y);
        T newW = Math::max(a.w, b.w);
        T newH = Math::max(a.h, b.h);
        return Rect<T>{newX, newY, Math::max(newW, (T)0.0), Math::max(newH, (T)0.0)};
    }

    template <typename T>
    Rect<T> Rect<T>::intersection(const Rect<T>& a, const Rect<T>& b) {
        T newX = Math::max(a.x, b.x);
        T newY = Math::max(a.y, b.y);
        T newW = Math::min(a.w, b.w);
        T newH = Math::min(a.h, b.h);
        return Rect<T>{newX, newY, Math::max(newW, (T)0.0), Math::max(newH, (T)0.0)};
    }

    template <typename T>
    void Rect<T>::swap(Rect<T>& first, Rect<T>& second) {
        using std::swap;

        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.w, second.w);
        swap(first.h, second.h);
    }
}  // namespace GLaDOS

#endif  //GLADOS_RECT_HPP
