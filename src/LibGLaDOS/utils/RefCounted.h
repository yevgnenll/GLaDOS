#ifndef GLADOS_REFCOUNTED_H
#define GLADOS_REFCOUNTED_H

#include <atomic>

namespace GLaDOS {
    class RefCounted {
        template <typename T>
        friend class RefPtr;

      public:
        RefCounted() = default;
        explicit constexpr RefCounted(int initialValue);

        bool isRefOne() const;
        bool isRefZero() const;

      private:
        int incrementRefOne();
        int incrementRef(int increment);
        bool releaseRef();

        std::atomic_int mRefCount{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_REFCOUNTED_H
