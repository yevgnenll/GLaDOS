#ifndef GLADOS_STLALLOCATOR_H
#define GLADOS_STLALLOCATOR_H

#include <limits>

#include "Allocation.h"

namespace GLaDOS {
    template <typename T>
    class STLAllocator {
      public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using const_reference = const T&;

        STLAllocator() = default;
        template<class U>
        constexpr STLAllocator(const STLAllocator<U>&) noexcept {}
        ~STLAllocator() = default;

        pointer allocate(size_type count);
        void deallocate(pointer ptr, size_type count) noexcept;
        size_type max_size() const;
        void construct(pointer p, const_reference val);
        void destroy(pointer ptr);
    };

    template<class T, class U>
    inline bool operator==(const STLAllocator<T>&, const STLAllocator<U>&) { return true; }

    template<class T, class U>
    inline bool operator!=(const STLAllocator<T>&, const STLAllocator<U>&) { return false; }

    template <typename T>
    typename STLAllocator<T>::pointer STLAllocator<T>::allocate(size_type count) {
        size_type size = count * sizeof(value_type);
        return static_cast<pointer>(MALLOC(size));
    }

    template <typename T>
    void STLAllocator<T>::deallocate(pointer ptr, [[maybe_unused]] size_type count) noexcept {
        FREE(ptr);
    }

    template <typename T>
    typename STLAllocator<T>::size_type STLAllocator<T>::max_size() const {
        return (std::numeric_limits<size_type>::max)() / sizeof(value_type);
    }

    template <typename T>
    void STLAllocator<T>::construct(pointer p, const_reference val) {
        new (static_cast<void*>(p)) T(val);
    }

    template <typename T>
    void STLAllocator<T>::destroy(pointer ptr) {
        ptr->~T();
    }
}  // namespace GLaDOS

#endif
