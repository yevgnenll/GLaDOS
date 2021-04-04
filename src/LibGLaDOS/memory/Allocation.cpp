#include "Allocation.h"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include "platform/OSTypes.h"
#include "utils/Utility.h"

namespace GLaDOS {
  void* mmalloc(std::size_t size, const char* file, int line, const char* function) {
    void* memory_ptr = nullptr;
#if defined(MSVC)
    memory_ptr = _aligned_malloc(size + sizeof(MemBlockDList), _mem_alignment);
    if (memory_ptr == nullptr) {
      printf("Allocation Failed");
      return memory_ptr;
    }
#else
    int result = posix_memalign(&memory_ptr, _mem_alignment, size + sizeof(MemBlockDList));
    if (result != 0) {
      printf("Allocation Failed\n");
      return memory_ptr;
    }
#endif
    MemBlockDList* memory_block = CAST(MemBlockDList*, memory_ptr);
    if (memory_block == nullptr) {
      return memory_block;
    }
    memory_block->file = file;
    memory_block->line = CAST(uint32_t, line);
    memory_block->function = function;
    memory_block->prev = nullptr;
    memory_block->size = size;

    {
      // synchronize block
      std::unique_lock<SpinLock> lock{_mem_spin_lock, std::try_to_lock};
      memory_block->next = _mem_block_head;
      if (_mem_block_head != nullptr) {
        memory_block->next->prev = memory_block;
      }
      _mem_block_head = memory_block;
    }

    printf("Memory allocated: file=%s, line=%d, function=%s, size=%ld\n", memory_block->file, memory_block->line, memory_block->function, memory_block->size);

    return memory_block + 1;
  }

  void mfree(void* ptr) {
    if (ptr == nullptr) {
      printf("Memory free Failed\n");
      return;
    }

    MemBlockDList* memory_block = CAST(MemBlockDList*, ptr) - 1;
    memory_block->size = ~memory_block->size;

    {
      // synchronize block
      std::unique_lock<SpinLock> lock{_mem_spin_lock, std::try_to_lock};
      if (memory_block->prev == nullptr) {
        assert(_mem_block_head == memory_block);
        _mem_block_head = memory_block->next;
      } else {
        memory_block->prev->next = memory_block->next;
      }

      if (memory_block->next != nullptr) {
        memory_block->next->prev = memory_block->prev;
      }
    }

#if defined(MSVC)
    _aligned_free(memory_block);
#else
    free(memory_block);
#endif
  }

  void mprint(const char* reason, MemBlockDList* mi) {
    printf("%s: %s: %s() (%4d): %zd bytes at %p\n", reason, mi->file, mi->function, mi->line, mi->size, (void*)(mi + 1));
  }

  void dumpMemory() {
    MemBlockDList* memory_block = _mem_block_head;
    bool leak = false;
    while (memory_block != nullptr) {
      if (CAST(ptrdiff_t, memory_block->size) >= 0) {
        if (!leak) {
          printf("[Memory Debug] Detected memory leaks!\n");
          leak = true;
        }
        mprint("Leaked", memory_block);
      }
      memory_block = memory_block->next;
    }

    if (!leak) {
      printf("[Memory Debug] No memory leaks.\n");
    }

    memory_block = _mem_block_head;
    while (memory_block != nullptr) {
      if ((ptrdiff_t)memory_block->size < 0) {
        mprint("Freed ", memory_block);
      }
      memory_block = memory_block->next;
    }
  }
}  // namespace GLaDOS