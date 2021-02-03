#ifndef GLADOS_BUFFER_H
#define GLADOS_BUFFER_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class Buffer {
  public:
    Buffer(BufferType type, BufferUsage usage);
    virtual ~Buffer() = default;

    std::size_t getSize() const;
    virtual bool uploadData(StreamBuffer& buffer) = 0;

  protected:
    BufferType mType;
    BufferUsage mUsage;
    std::size_t mSize{0};
  };
}  // namespace GLaDOS

#endif
