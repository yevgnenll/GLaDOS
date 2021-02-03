#ifndef GLADOS_ALLOCATION_H
#define GLADOS_ALLOCATION_H

#include <cassert>
#include <cstddef>
#include <cstdlib>

#include "Config.h"

namespace GLaDOS {
  struct MemoryHeader {
    const char* file;
    int line;
    const char* function;
    size_t size;
    MemoryHeader *next, *prev;
  };

  static MemoryHeader* headOfMemory;

  extern void* mmalloc(size_t sz, const char* file, int line, const char* function);
  extern void mfree(void* ptr);
  extern void mprint(const char* reason, MemoryHeader* mi);
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
    for (size_t i = 0; i < count; i++) { \
      (ptr)[i]->~T(); \
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
