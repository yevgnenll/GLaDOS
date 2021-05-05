#ifndef GLADOS_THREADLOCALFIXEDSIZEMEMORYPOOL_H
#define GLADOS_THREADLOCALFIXEDSIZEMEMORYPOOL_H

#include "FixedSizeMemoryPool.hpp"
#include "utils/ThreadLocalStorage.hpp"

namespace GLaDOS {
  // Thread local fixed size memory pool allocator (lock-free).
  template <typename T, typename Allocator = STLAllocator<AlignedMemBlock<T>>>
  class ThreadLocalAllocator {
    using AllocatorType = ThreadLocalStore<FixedSizeMemoryPool<T, Allocator>>;

  public:
    ThreadLocalAllocator() = default;
    ~ThreadLocalAllocator() = default;

    DISALLOW_COPY_AND_ASSIGN(ThreadLocalAllocator);

    template <typename... Args>
    T* allocate(Args&&... args);
    void deallocate(T* ptr);
  };

  template <typename T, typename Allocator>
  template <typename... Args>
  T* ThreadLocalAllocator<T, Allocator>::allocate(Args&&... args) {
    return AllocatorType::get().allocate(std::forward<Args>(args)...);
  }

  template <typename T, typename Allocator>
  void ThreadLocalAllocator<T, Allocator>::deallocate(T* ptr) {
    AllocatorType::get().deallocate(ptr);
  }
}

#endif  //GLADOS_THREADLOCALALLOCATOR_H
