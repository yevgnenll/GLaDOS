#include "Allocation.h"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include "utils/Utility.h"

namespace GLaDOS {
    std::size_t alignment(std::size_t operand, std::size_t alignment) {
        return (operand + (alignment - 1)) & ~(alignment - 1);
    }

    void* align_malloc(std::size_t size, std::size_t alignment) {
        void* memory_ptr = nullptr;
#if defined(MSVC)
        memory_ptr = _aligned_malloc(size, alignment);
#else
        int result = posix_memalign(&memory_ptr, alignment, size);
#endif
        return memory_ptr;
    }

    void align_free(void* pointer) {
#if defined(MSVC)
        _aligned_free(pointer);
#else
        free(pointer);
#endif
    }

    void* mmalloc(std::size_t size, const char* file, int line, const char* function) {
        void* memory_ptr = align_malloc(size + sizeof(MemBlockDList), _mem_alignment);
        if (memory_ptr == nullptr) {
#ifdef MEMORY_DEBUG_PRINT
            printf("Allocation failed : %s: %s() (%4d): %zd bytes\n", file, function, line, size);
#endif
            return memory_ptr;
        }
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

#ifdef MEMORY_DEBUG_PRINT
        mprint("Allocated ", memory_block);
#endif

        return memory_block + 1;
    }

    void mfree(void* ptr) {
        if (ptr == nullptr) {
#ifdef MEMORY_DEBUG_PRINT
            printf("Free Failed\n");
#endif
            return;
        }

        MemBlockDList* memory_block = CAST(MemBlockDList*, ptr) - 1;
#ifdef MEMORY_DEBUG_PRINT
        mprint("Freed ", memory_block);
#endif
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

        align_free(memory_block);
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
                mprint("Leaked ", memory_block);
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