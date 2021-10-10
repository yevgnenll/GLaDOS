#include "IndexBuffer.h"

namespace GLaDOS {
    IndexBuffer::IndexBuffer(std::size_t stride, std::size_t count, bool allocate) {
        mStride = stride;
        mSize = count * mStride;
        if (allocate) {
            this->allocate();
        }
    }
}  // namespace GLaDOS