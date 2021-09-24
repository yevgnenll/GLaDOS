#ifndef GLADOS_REFPTR_HPP
#define GLADOS_REFPTR_HPP

#include "RefCounted.h"
#include "memory/Allocation.h"

namespace GLaDOS {
    // Reference-counting shared smart pointer for RefCounted class.
    template <typename T>
    class RefPtr {
      public:
        RefPtr() = default;
        RefPtr(T* pointer); // non-explicit intentionally
        ~RefPtr();

        RefPtr(const RefPtr<T>& other);
        RefPtr<T>& operator=(const RefPtr<T>& other);

        template <typename TCastFrom>
        RefPtr(const RefPtr<TCastFrom>& castFrom); // non-explicit intentionally
        template <typename TCastFrom>
        RefPtr<T>& operator=(const RefPtr<TCastFrom>& castFrom);

        T* operator->() const;
        T& operator*() const;
        bool operator==(const RefPtr<T>& other) const;
        bool operator!=(const RefPtr<T>& other) const;
        bool operator<(const RefPtr<T>& other) const;
        bool operator<=(const RefPtr<T>& other) const;
        bool operator>(const RefPtr<T>& other) const;
        bool operator>=(const RefPtr<T>& other) const;
        operator bool() const;

        T* get() const;
        void reset(T* pointer);
        void reset(const RefPtr<T>& other);
        bool isPresent() const;
        RefPtr<T>& swap(RefPtr<T>& other);

        template <typename TCastFrom>
        static RefPtr<T> cast_dynamic(const RefPtr<TCastFrom>& other);
        template <typename TCastFrom>
        static RefPtr<T> cast_static(const RefPtr<TCastFrom>& other);
        template <typename TCastFrom>
        static RefPtr<T> cast_const(const RefPtr<TCastFrom>& other);

      private:
        void destroy();

      private:
        T* mPointer{nullptr};
    };

    template <typename T>
    RefPtr<T>::RefPtr(T* pointer) : mPointer{pointer} {
        if (mPointer) {
            mPointer->incrementRefOne();
        }
    }

    template <typename T>
    RefPtr<T>::~RefPtr() {
        destroy();
    }

    template <typename T>
    RefPtr<T>::RefPtr(const RefPtr<T>& other) : mPointer{other.mPointer} {
        if (mPointer) {
            mPointer->incrementRefOne();
        }
    }

    template <typename T>
    RefPtr<T>& RefPtr<T>::operator=(const RefPtr<T>& other) {
        T* oldPointer = mPointer;
        mPointer = other.mPointer;

        if (mPointer) {
            mPointer->incrementRefOne();
        }

        if (oldPointer && oldPointer->releaseRef()) {
            DELETE_T(oldPointer, T);
        }

        return *this;
    }

    template <typename T>
    template <typename TCastFrom>
    RefPtr<T>::RefPtr(const RefPtr<TCastFrom>& castFrom) : mPointer{castFrom.get()} {
        if (mPointer) {
            mPointer->incrementRefOne();
        }
    }

    template <typename T>
    template <typename TCastFrom>
    RefPtr<T>& RefPtr<T>::operator=(const RefPtr<TCastFrom>& castFrom) {
        T* oldPointer = mPointer;
        mPointer = castFrom.get();

        if (mPointer) {
            mPointer->incrementRefOne();
        }

        if (oldPointer && oldPointer->releaseRef()) {
            DELETE_T(oldPointer, T);
        }

        return *this;
    }

    template <typename T>
    T* RefPtr<T>::operator->() const {
        return mPointer;
    }

    template <typename T>
    T& RefPtr<T>::operator*() const {
        return *mPointer;
    }

    template <typename T>
    bool RefPtr<T>::operator==(const RefPtr<T>& other) const {
        return (mPointer == other.mPointer);
    }

    template <typename T>
    bool RefPtr<T>::operator!=(const RefPtr<T>& other) const {
        return (mPointer != other.mPointer);
    }

    template <typename T>
    bool RefPtr<T>::operator<(const RefPtr<T>& other) const {
        return (mPointer < other.mPointer);
    }

    template <typename T>
    bool RefPtr<T>::operator<=(const RefPtr<T>& other) const {
        return (mPointer <= other.mPointer);
    }

    template <typename T>
    bool RefPtr<T>::operator>(const RefPtr<T>& other) const {
        return (mPointer > other.mPointer);
    }

    template <typename T>
    bool RefPtr<T>::operator>=(const RefPtr<T>& other) const {
        return (mPointer >= other.mPointer);
    }

    template <typename T>
    RefPtr<T>::operator bool() const {
        return (mPointer != 0);
    }

    template <typename T>
    T* RefPtr<T>::get() const {
        return mPointer;
    }

    template <typename T>
    void RefPtr<T>::reset(T* pointer) {
        if (pointer == nullptr) return;
        T* oldPointer = mPointer;
        mPointer = pointer;

        if (mPointer) {
            mPointer->incrementRefOne();
        }

        if (oldPointer && oldPointer->releaseRef()) {
            DELETE_T(oldPointer, T);
        }
    }

    template <typename T>
    void RefPtr<T>::reset(const RefPtr<T>& other) {
        if (other.mPointer == nullptr) return;
        T* oldPointer = mPointer;
        mPointer = other.mPointer;

        if (mPointer) {
            mPointer->incrementRefOne();
        }

        if (oldPointer && oldPointer->releaseRef()) {
            DELETE_T(oldPointer, T);
        }
    }

    template <typename T>
    void RefPtr<T>::destroy() {
        if (mPointer && mPointer->releaseRef()) {
            DELETE_T(mPointer, T);
        }
    }

    template <typename T>
    bool RefPtr<T>::isPresent() const {
        return mPointer != nullptr;
    }

    template <typename T>
    RefPtr<T>& RefPtr<T>::swap(RefPtr<T>& other) {
        T* const temp = mPointer;
        mPointer = other.mPointer;
        other.mPointer = temp;
    }

    template <typename T>
    template <typename TCastFrom>
    RefPtr<T> RefPtr<T>::cast_dynamic(const RefPtr<TCastFrom>& other) {
        T* const pointer = dynamic_cast<T*>(other.get());
        return RefPtr<T>(pointer);
    }

    template <typename T>
    template <typename TCastFrom>
    RefPtr<T> RefPtr<T>::cast_static(const RefPtr<TCastFrom>& other) {
        T* const pointer = static_cast<T*>(other.get());
        return RefPtr<T>(pointer);
    }

    template <typename T>
    template <typename TCastFrom>
    RefPtr<T> RefPtr<T>::cast_const(const RefPtr<TCastFrom>& other) {
        T* const pointer = const_cast<T*>(other.get());
        return RefPtr<T>(pointer);
    }
}  // namespace GLaDOS

#endif  //GLADOS_REFPTR_HPP
