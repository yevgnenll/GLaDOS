#ifndef GAMEENGINE_SPINLOCK_H
#define GAMEENGINE_SPINLOCK_H

#include <atomic>

namespace GameEngine {
  class SpinLock {
  public:
    SpinLock() = default;

    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

  private:
    std::atomic_bool mAtomicLock{false};
  };
}  // namespace GameEngine

#endif  //GAMEENGINE_SPINLOCK_H
