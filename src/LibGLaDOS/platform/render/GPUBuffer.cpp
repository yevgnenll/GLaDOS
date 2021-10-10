#include "GPUBuffer.h"

namespace GLaDOS {
    GPUBuffer::GPUBuffer(GPUBufferType type, GPUBufferUsage usage)
        : mType{type}, mUsage{usage} {
    }

    std::size_t GPUBuffer::getSize() const {
        return mSize;
    }
}  // namespace GLaDOS