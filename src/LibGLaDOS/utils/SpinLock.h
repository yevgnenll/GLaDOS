#ifndef GLADOS_SPINLOCK_H
#define GLADOS_SPINLOCK_H

#include <atomic>

namespace GLaDOS {
    class SpinLock {
      public:
        SpinLock() = default;

        void lock() noexcept;
        bool try_lock() noexcept;
        void unlock() noexcept;

      private:
        std::atomic_bool mAtomicLock{false};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPINLOCK_H
