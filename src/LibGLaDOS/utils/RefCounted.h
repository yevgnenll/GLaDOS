#ifndef GLADOS_REFCOUNTED_H
#define GLADOS_REFCOUNTED_H

#include <atomic>

namespace GLaDOS {
  class RefCounted {
  public:
    RefCounted() = default;
    explicit constexpr RefCounted(int initialValue);

    int incrementRefOne();
    int incrementRef(int increment);
    bool releaseRef();
    bool isRefOne() const;
    bool isRefZero() const;

  private:
    std::atomic_int mRefCount{0};
  };
}

#endif  //GLADOS_REFCOUNTED_H
