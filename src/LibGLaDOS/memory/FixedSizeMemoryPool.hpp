#ifndef GLADOS_FIXEDSIZEMEMORYPOOL_HPP
#define GLADOS_FIXEDSIZEMEMORYPOOL_HPP

#include "utils/Utility.h"

namespace GLaDOS {
    template <typename T>
    struct AlignedMemBlock {
        using AlignedSizeType = std::aligned_storage_t<sizeof(T), alignof(T)>;
        AlignedSizeType value;
        AlignedMemBlock* next;
    };

    template <typename T, typename Allocator = STLAllocator<AlignedMemBlock<T>>>
    class FixedSizeMemoryPool {
      public:
        using MemBlockType = AlignedMemBlock<T>;
        static constexpr std::size_t defaultPoolSize = ((1 << 22) / sizeof(T));

        FixedSizeMemoryPool(std::size_t poolSize = defaultPoolSize) noexcept;
        ~FixedSizeMemoryPool() noexcept;

        template <typename... Args>
        T* allocate(Args&&... args);
        void deallocate(T* ptr);

        DISALLOW_COPY_AND_ASSIGN(FixedSizeMemoryPool);

      private:
        Allocator* mAllocator{nullptr};
        MemBlockType* mHead{nullptr};
        MemBlockType* mData{nullptr};
        std::size_t mSize;
    };

    template <typename T, typename Allocator>
    FixedSizeMemoryPool<T, Allocator>::FixedSizeMemoryPool(std::size_t poolSize) noexcept : mSize{poolSize} {
        mAllocator = NEW_T(Allocator);
        mData = mAllocator->allocate(mSize);
        mHead = mData;

        for (std::size_t i = 0; i < mSize; i++) {
            mData[i].next = std::addressof(mData[i + 1]);
        }
        mData[mSize - 1].next = nullptr;
    }

    template <typename T, typename Allocator>
    FixedSizeMemoryPool<T, Allocator>::~FixedSizeMemoryPool() noexcept {
        mAllocator->deallocate(mData, mSize);
        DELETE_T(mAllocator, Allocator);
        mHead = nullptr;
        mData = nullptr;
    }

    template <typename T, typename Allocator>
    template <typename... Args>
    T* FixedSizeMemoryPool<T, Allocator>::allocate(Args&&... args) {
        if (mHead == nullptr) {
            return nullptr;
        }

        MemBlockType* poolBlock = mHead;
        mHead = mHead->next;
        return new (std::addressof(poolBlock->value)) T(std::forward<Args>(args)...);
    }

    template <typename T, typename Allocator>
    void FixedSizeMemoryPool<T, Allocator>::deallocate(T* ptr) {
        ptr->~T();
        MemBlockType* poolBlock = reinterpret_cast<MemBlockType*>(ptr);
        poolBlock->next = mHead;
        mHead = poolBlock;
    }
}  // namespace GLaDOS

#endif  //GLADOS_FIXEDSIZEMEMORYPOOL_HPP
