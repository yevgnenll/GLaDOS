#include "IndexBuffer.h"

namespace GLaDOS {
    IndexBuffer::IndexBuffer(std::size_t stride, std::size_t count) {
        mStride = stride;
        mSize = count * mStride;
        mBufferData.resize(mSize);
    }

    void IndexBuffer::copyBufferData(void *data) {
        mBufferData.copyFrom(reinterpret_cast<std::byte*>(data), mSize);
    }
}  // namespace GLaDOS