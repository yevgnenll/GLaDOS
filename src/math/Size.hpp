#ifndef GLADOS_SIZE_HPP
#define GLADOS_SIZE_HPP

namespace GLaDOS {
  template <typename T>
  class Size {
  public:
    Size();
    ~Size() = default;
    Size(const T& _x, const T& _y);
    Size(const Size<T>& other) = default;
    Size(Size<T>&& other) noexcept;

    Size<T>& operator=(Size<T> other);
    bool operator==(const Size<T>& other) const;
    bool operator!=(const Size<T>& other) const;

    T x, y;

  private:
    void swap(Size<T>& first, Size<T>& second);
  };

  template <typename T>
  Size<T>::Size() : x{(T)0}, y{(T)0} {

  }

  template <typename T>
  Size<T>::Size(const T& _x, const T& _y) : x{(T)_x}, y{(T)_y} {
  }

  template <typename T>
  Size<T>::Size(Size<T>&& other) noexcept : Size<T>{} {
    Size<T>::swap(*this, other);
  }

  template <typename T>
  Size<T>& Size<T>::operator=(Size<T> other) {
    // copy and swap idiom (effective c++ section 11)
    Size<T>::swap(*this, other);
    return *this;
  }

  template <typename T>
  bool Size<T>::operator==(const Size<T>& other) const {
    return x == other.x && y == other.y;
  }

  template <typename T>
  bool Size<T>::operator!=(const Size<T>& other) const {
    return !(*this == other);
  }

  template <typename T>
  void Size<T>::swap(Size<T>& first, Size<T>& second) {
    using std::swap;

    swap(first.x, second.x);
    swap(first.y, second.y);
  }
}

#endif  //GLADOS_SIZE_HPP
