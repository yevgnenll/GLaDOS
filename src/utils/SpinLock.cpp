#include "SpinLock.h"

namespace GameEngine {
  void SpinLock::lock() noexcept {
    for (;;) {
      // Optimistically assume the lock is free on the first try
      if (!mAtomicLock.exchange(true, std::memory_order_acquire)) {
        return;
      }
      // Wait for lock to be released without generating cache misses
      while (mAtomicLock.load(std::memory_order_relaxed)) {}
    }
  }

  bool SpinLock::try_lock() noexcept {
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !mAtomicLock.load(std::memory_order_relaxed) && !mAtomicLock.exchange(true, std::memory_order_acquire);
  }

  void SpinLock::unlock() noexcept {
    mAtomicLock.store(false, std::memory_order_release);
  }
}