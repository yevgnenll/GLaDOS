#include "GPUBuffer.h"

namespace GLaDOS {
  GPUBuffer::GPUBuffer(BufferType type, BufferUsage usage)
      : mType{type}, mUsage{usage} {
  }

  std::size_t GPUBuffer::getSize() const {
    return mSize;
  }
}  // namespace GLaDOS