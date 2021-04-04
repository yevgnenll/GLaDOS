#ifndef GLADOS_FIXEDSIZEMEMORYPOOL_HPP
#define GLADOS_FIXEDSIZEMEMORYPOOL_HPP

#include "utils/Singleton.hpp"
#include "utils/SpinLock.h"
#include "utils/Utility.h"

namespace GLaDOS {
  template <typename T>
  class FixedSizeMemoryPool : public Singleton<FixedSizeMemoryPool<T>> {
  public:
    FixedSizeMemoryPool() noexcept;
    ~FixedSizeMemoryPool() noexcept;

    template <typename... Args>
    T* allocate(Args&&... args);
    void deallocate(T* ptr);

    DISALLOW_COPY_AND_ASSIGN(FixedSizeMemoryPool);

  private:
    void allocateBlock() noexcept;

    struct FixedSizeLinkedList {
      T value;
      FixedSizeLinkedList* next;
    };

    FixedSizeLinkedList* mMemHead;
    SpinLock mMemAllocSpinLock;
    Vector<void*> mFixedBlockPointers;

    constexpr static std::size_t fixed_block_size = 1 << 12;  // fixed size page defined to 4KB
  };

  template <typename T>
  FixedSizeMemoryPool<T>::FixedSizeMemoryPool() {
  }

  template <typename T>
  FixedSizeMemoryPool<T>::~FixedSizeMemoryPool() {
  }

  template <typename T>
  template <typename... Args>
  T* FixedSizeMemoryPool<T>::allocate(Args&&... args) {
  }

  template <typename T>
  void FixedSizeMemoryPool<T>::deallocate(T* ptr) {
  }

  template <typename T>
  void FixedSizeMemoryPool<T>::allocateBlock() {
  }
}  // namespace GLaDOS

#endif  //GLADOS_FIXEDSIZEMEMORYPOOL_HPP
