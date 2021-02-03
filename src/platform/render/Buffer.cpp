#include "Buffer.h"

namespace GLaDOS {
  Buffer::Buffer(BufferType type, BufferUsage usage)
      : mType{type}, mUsage{usage} {
  }

  std::size_t Buffer::getSize() const {
    return mSize;
  }
}  // namespace GLaDOS