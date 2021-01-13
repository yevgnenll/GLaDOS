#ifndef GAMEENGINE_BUFFER_H
#define GAMEENGINE_BUFFER_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GameEngine {
  class Buffer {
  public:
    Buffer(BufferType type, BufferUsage usage, const StreamBuffer& buffer);
    virtual ~Buffer() = default;

    std::size_t getSize() const;
    virtual bool uploadData(StreamBuffer& buffer) = 0;

  private:
    BufferType mType;
    BufferUsage mUsage;
    std::size_t mSize;
  };
}  // namespace GameEngine

#endif
