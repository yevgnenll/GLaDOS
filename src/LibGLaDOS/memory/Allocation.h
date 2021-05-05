#ifndef GLADOS_ALLOCATION_H
#define GLADOS_ALLOCATION_H

#include "Config.h"
#include "utils/SpinLock.h"

namespace GLaDOS {
  struct MemBlockDList {
    const char* file;
    uint32_t line;
    const char* function;
    std::size_t size;
    MemBlockDList *next, *prev;
  };

  static MemBlockDList* _mem_block_head;
  static SpinLock _mem_spin_lock;
  constexpr static std::size_t _mem_alignment = 0x0040;  // 64 default cache line size
  constexpr static std::size_t _gpu_mem_alignment = 0x1000; // 4096

  extern std::size_t alignment(std::size_t operand, std::size_t alignment);
  extern void* align_malloc(std::size_t size, std::size_t alignment);
  extern void align_free(void* pointer);
  extern void* mmalloc(std::size_t size, const char* file, int line, const char* function);
  extern void mfree(void* ptr);
  extern void mprint(const char* reason, MemBlockDList* mi);
  extern void dumpMemory();
}  // namespace GLaDOS

#if MEMORY_DEBUG == 1
#define MALLOC(bytes) GLaDOS::mmalloc(bytes, __FILE__, __LINE__, __FUNCTION__)
#define FREE(ptr) GLaDOS::mfree(static_cast<void*>(ptr))
#define NEW_T(T) new (GLaDOS::mmalloc(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
#define DELETE_POD(ptr) \
  if (ptr) { \
    FREE(ptr); \
    (ptr) = nullptr; \
  }
#define DELETE_T(ptr, T) \
  if (ptr) { \
    (ptr)->~T(); \
    FREE(ptr); \
    (ptr) = nullptr; \
  }
#define NEW_ARRAY_T(T, count) new (GLaDOS::mmalloc(sizeof(T) * (count), __FILE__, __LINE__, __FUNCTION__)) T
#define DELETE_ARRAY_T(ptr, T, count) \
  if (ptr) { \
    for (std::size_t i = 0; i < count; i++) { \
      (ptr)[i].~T(); \
    } \
    GLaDOS::mfree(static_cast<void*>(ptr)); \
  }
#else
#define MALLOC(bytes) std::malloc(bytes)
#define FREE(ptr) std::free(ptr)
#define NEW_T(T) new T
#define DELETE_POD(ptr) \
  if (ptr) { \
    delete (ptr); \
    (ptr) = nullptr; \
  }
#define DELETE_T(ptr, T) DELETE_POD(ptr)
#define NEW_ARRAY_T(T, count) new T[count]
#define DELETE_ARRAY_T(ptr, T, count) \
  if (ptr) { \
    delete[](ptr); \
    (ptr) = nullptr; \
  }
#endif

#endif
