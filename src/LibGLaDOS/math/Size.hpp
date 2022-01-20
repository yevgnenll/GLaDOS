#ifndef GLADOS_SIZE_HPP
#define GLADOS_SIZE_HPP

#include <utility>

namespace GLaDOS {
    template <typename T>
    class Size {
      public:
        Size();
        ~Size() = default;
        Size(const T& _w, const T& _h);
        Size(const Size<T>& other) = default;
        Size(Size<T>&& other) noexcept;

        Size<T>& operator=(Size<T> other);
        bool operator==(const Size<T>& other) const;
        bool operator!=(const Size<T>& other) const;
        T area() const;

        T w, h;

      private:
        void swap(Size<T>& first, Size<T>& second);
    };

    template <typename T>
    Size<T>::Size() : w{(T)0}, h{(T)0} {
    }

    template <typename T>
    Size<T>::Size(const T& _w, const T& _h) : w{(T)_w}, h{(T)_h} {
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
        return w == other.w && h == other.h;
    }

    template <typename T>
    bool Size<T>::operator!=(const Size<T>& other) const {
        return !(*this == other);
    }

    template <typename T>
    T Size<T>::area() const {
        return w * h;
    }

    template <typename T>
    void Size<T>::swap(Size<T>& first, Size<T>& second) {
        using std::swap;

        swap(first.w, second.w);
        swap(first.h, second.h);
    }

    using Sizei = Size<std::size_t>;
}  // namespace GLaDOS

#endif  //GLADOS_SIZE_HPP
