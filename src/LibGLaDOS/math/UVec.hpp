#ifndef GLADOS_UVEC_HPP
#define GLADOS_UVEC_HPP

#include <cstddef>

namespace GLaDOS {
    template <typename T, std::size_t N>
    class UVec {
      public:
        // should not be explicit
        operator const Vec<T, N>&() const;
        const Vec<T, N>* operator->() const;
        const Vec<T, N>& operator*() const;
        ~UVec3() = default;

      private:
        friend class Vec<T, N>;
        explicit UVec(const Vec<T, N>& v);
        Vec<T, N> mVec;
    };

    template <typename T, std::size_t N>
    UVec<T, N>::UVec(const Vec<T, N>& v) : mVec{v} {
    }

    template <typename T, std::size_t N>
    UVec<T, N>::operator const Vec<T, N>&() const {
        return mVec;
    }

    template <typename T, std::size_t N>
    const Vec<T, N>* UVec<T, N>::operator->() const {
        return &mVec;
    }

    template <typename T, std::size_t N>
    const Vec<T, N>& UVec<T, N>::operator*() const {
        return mVec;
    }
}

#endif  // GLADOS_UVEC_HPP
