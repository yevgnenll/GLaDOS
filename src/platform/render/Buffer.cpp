#include "Buffer.h"

namespace GameEngine {
  Buffer::Buffer(BufferType type, BufferUsage usage, const StreamBuffer& buffer)
      : mType{type}, mUsage{usage}, mSize{buffer.size()} {
  }

  std::size_t Buffer::getSize() const {
    return mSize;
  }
}  // namespace GameEngine