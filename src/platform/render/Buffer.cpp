#include "Buffer.h"

namespace GameEngine {
  Buffer::Buffer(BufferType type, BufferUsage usage) : mType{type}, mUsage{usage} {
  }

  Buffer::~Buffer() {
  }

  uint32_t Buffer::getSize() const {
    return mSize;
  }
}  // namespace GameEngine