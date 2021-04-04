#include "RefCounted.h"

namespace GLaDOS {
  constexpr RefCounted::RefCounted(int initialValue) : mRefCount{initialValue} {
  }

  int RefCounted::incrementRefOne() {
    return incrementRef(1);
  }

  int RefCounted::incrementRef(int increment) {
    return mRefCount.fetch_add(increment, std::memory_order_relaxed);
  }

  bool RefCounted::releaseRef() {
    return mRefCount.fetch_sub(1, std::memory_order_acq_rel) == 0;
  }

  bool RefCounted::isRefOne() const {
    return mRefCount.load(std::memory_order_acquire) == 1;
  }

  bool RefCounted::isRefZero() const {
    return mRefCount.load(std::memory_order_acquire) == 0;
  }
}