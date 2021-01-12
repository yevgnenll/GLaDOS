#ifndef GAMEENGINE_BUFFER_H
#define GAMEENGINE_BUFFER_H

#include "utils/Enumeration.h"

namespace GameEngine {
  class Buffer {
  public:
    Buffer(BufferType type, BufferUsage usage);
    virtual ~Buffer();

    uint32_t getSize() const;
    virtual bool uploadData() = 0;

  private:
    BufferType mType;
    BufferUsage mUsage;
    uint32_t mSize;
  };
}  // namespace GameEngine

#endif
