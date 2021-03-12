#ifndef GLADOS_RECT_HPP
#define GLADOS_RECT_HPP

#include "Math.h"
#include "Size.hpp"

namespace GLaDOS {
  class Vec2;
  template <typename T>
  class Rect {
  public:
    Rect();
    Rect(const T& _left, const T& _top, const T& _right, const T& _bottom);
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
    Rect<T>& setRect(const T& _left, const T& _top, const T& _right, const T& _bottom);
    T area() const;
    void makeExpand(T to);

    // anonymous union
    union {
      struct {
        T left, top;
        T right, bottom;
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
  Rect<T>::Rect() : left{(T)0}, top{(T)0}, right{(T)0}, bottom{(T)0} {
  }

  template <typename T>
  Rect<T>::Rect(const T& _left, const T& _top, const T& _right, const T& _bottom) : left{_left}, top{_top}, right{_right}, bottom{_bottom} {
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
    return left == other.left && right == other.right && top == other.top && bottom == other.bottom;
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
    left += other.left;
    top += other.top;
    right += other.right;
    bottom += other.bottom;
    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator-(const Rect<T>& other) const {
    return Rect<T>(*this) -= other;
  }

  template <typename T>
  Rect<T>& Rect<T>::operator-=(const Rect<T>& other) {
    left -= other.left;
    top -= other.top;
    right -= other.right;
    bottom -= other.bottom;
    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator*(const Rect<T>& other) const {
    return Rect<T>(*this) *= other;
  }

  template <typename T>
  Rect<T>& Rect<T>::operator*=(const Rect<T>& other) {
    left *= other.left;
    top *= other.top;
    right *= other.right;
    bottom *= other.bottom;
    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator/(const Rect<T>& other) const {
    return Rect<T>(*this) /= other;
  }

  template <typename T>
  Rect<T>& Rect<T>::operator/=(const Rect<T>& other) {
    left /= other.left;
    top /= other.top;
    right /= other.right;
    bottom /= other.bottom;
    return *this;
  }

  template <typename T>
  Size<T> Rect<T>::toSize() const {
    return Size<T>{right - left, bottom - top};
  }

  template <typename T>
  Rect<T>& Rect<T>::makeMerge(const Rect<T>& other) {
    left = Math::min(left, other.left);
    right = Math::max(right, other.right);
    top = Math::min(top, other.top);
    bottom = Math::max(bottom, other.bottom);
    return *this;
  }

  template <typename T>
  bool Rect<T>::intersect(const Rect<T>& other) const {
    Rect rect;
    rect.left = Math::max(left, other.left);
    rect.right = Math::min(right, other.right);
    rect.top = Math::max(top, other.top);
    rect.bottom = Math::min(bottom, other.bottom);
    return !static_cast<bool>(rect.left > rect.right || rect.top > rect.bottom);
  }

  template <typename T>
  bool Rect<T>::contains(const Vec2& value) const {
    return !static_cast<bool>(left > value.x || right <= value.x || top > value.y || bottom <= value.y);
  }

  template <typename T>
  bool Rect<T>::contains(const Rect<T>& value) const {
    Size<T> thisSize = toSize();
    Size<T> otherSize = value.toSize();
    return left <= value.left && otherSize.x <= thisSize.x && top <= value.top && otherSize.y <= thisSize.y;
  }

  template <typename T>
  Rect<T>& Rect<T>::setRect(const T& _left, const T& _top, const T& _right, const T& _bottom) {
    return *this = Rect<T>{_left, _top, _right, _bottom};
  }

  template <typename T>
  T Rect<T>::area() const {
    return (right - left) * (bottom - top);
  }

  template <typename T>
  void Rect<T>::makeExpand(T to) {
    left -= to * 0.5;
    top -= to * 0.5;
    right += to;
    bottom += to;
  }

  template <typename T>
  Rect<T> Rect<T>::expand(Rect<T>& rect, T to) {
    rect.left -= to * 0.5;
    rect.top -= to * 0.5;
    rect.right += to;
    rect.bottom += to;
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
    T left = Math::min(a.left, b.left);
    T right = Math::max(a.right, b.right);
    T top = Math::min(a.top, b.top);
    T bottom = Math::max(a.bottom, b.bottom);
    return Rect<T>{left, top, Math::max(right, 0.0), Math::max(bottom, 0.0)};
  }

  template <typename T>
  Rect<T> Rect<T>::intersection(const Rect<T>& a, const Rect<T>& b) {
    T left = Math::max(a.left, b.left);
    T right = Math::min(a.right, b.right);
    T top = Math::max(a.top, b.top);
    T bottom = Math::min(a.bottom, b.bottom);
    return Rect<T>{left, top, Math::max(right, 0.0), Math::max(bottom, 0.0)};
  }

  template <typename T>
  void Rect<T>::swap(Rect<T>& first, Rect<T>& second) {
    using std::swap;

    swap(first.left, second.left);
    swap(first.top, second.top);
    swap(first.right, second.right);
    swap(first.bottom, second.bottom);
  }
}  // namespace GLaDOS

#endif  //GLADOS_RECT_HPP
