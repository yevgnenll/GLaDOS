#ifndef GAMEENGINE_ALLOCATION_H
#define GAMEENGINE_ALLOCATION_H

#include <cassert>
#include <cstddef>
#include <cstdlib>

#define MEMORY_DEBUG

namespace GameEngine {
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
}  // namespace GameEngine

#ifdef MEMORY_DEBUG
#define MALLOC(bytes) GameEngine::mmalloc(bytes, __FILE__, __LINE__, __FUNCTION__)
#define FREE(ptr) GameEngine::mfree(static_cast<void*>(ptr))
#define NEW_T(T) new (GameEngine::mmalloc(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
#define DELETE_T(ptr, T) \
  if (ptr) { \
    (ptr)->~T(); \
    FREE(ptr); \
    ptr = nullptr; \
  }
#define NEW_ARRAY_T(T, count) new (GameEngine::mmalloc(sizeof(T) * (count), __FILE__, __LINE__, __FUNCTION__)) T
#define DELETE_ARRAY_T(ptr, T, count) \
  if (ptr) { \
    for (size_t i = 0; i < count; i++) { \
      (ptr)[i]->~T(); \
    } \
    GameEngine::mfree(static_cast<void*>(ptr)); \
  }
#else
#define MALLOC(bytes) std::malloc(bytes)
#define FREE(ptr) std::free(ptr)
#define NEW_T(T) new T
#define DELETE_T(ptr, T) \
  if (ptr) { \
    delete (ptr); \
    (ptr) = nullptr; \
  }
#define NEW_ARRAY_T(T, count) new T[count]
#define DELETE_ARRAY_T(ptr, T, count) \
  if (ptr) { \
    delete[](ptr); \
    (ptr) = nullptr; \
  }
#endif

#endif
