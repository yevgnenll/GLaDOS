#include "Allocation.h"

#include <cstdio>

#include "utils/Utility.h"

namespace GLaDOS {
  // TODO: not thread safe
  void* mmalloc(size_t size, const char* file, int line, const char* function) {
    auto* mi = static_cast<MemoryHeader*>(malloc(size + sizeof(MemoryHeader)));
    if (mi == nullptr) {
      return mi;
    }
    mi->file = file;
    mi->line = line;
    mi->function = function;
    mi->next = headOfMemory;
    if (headOfMemory != nullptr) {
      mi->next->prev = mi;
    }
    mi->prev = nullptr;
    mi->size = size;
    headOfMemory = mi;
    return mi + 1;
  }

  void mfree(void* ptr) {
    if (ptr != nullptr) {
      MemoryHeader* mi = static_cast<MemoryHeader*>(ptr) - 1;
      mi->size = ~mi->size;
      if (mi->prev == nullptr) {
        assert(headOfMemory == mi);
        headOfMemory = mi->next;
      } else {
        mi->prev->next = mi->next;
      }

      if (mi->next != nullptr) {
        mi->next->prev = mi->prev;
      }
      free(mi);
    }
  }

  void mprint(const char* reason, MemoryHeader* mi) {
    printf("%s: %s: %s() (%4d): %zd bytes at %p\n", reason, mi->file, mi->function, mi->line, mi->size, (void*)(mi + 1));
  }

  void dumpMemory() {
    MemoryHeader* mi = headOfMemory;
    bool leak = false;
    while (mi != nullptr) {
      if ((ptrdiff_t)mi->size >= 0) {
        if (!leak) {
          LOG_TRACE("[Memory Debug] Detected memory leaks!\n");
          leak = true;
        }
        mprint("Leaked", mi);
      }
      mi = mi->next;
    }

    if (!leak) {
      LOG_TRACE("[Memory Debug] No memory leaks.\n");
    }

    mi = headOfMemory;
    while (mi != nullptr) {
      if ((ptrdiff_t)mi->size < 0) {
        mprint("Freed ", mi);
      }
      mi = mi->next;
    }
  }
}  // namespace GLaDOS