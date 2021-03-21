#ifndef GLADOS_GPUBUFFER_H
#define GLADOS_GPUBUFFER_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class GPUBuffer {
  public:
    GPUBuffer(BufferType type, BufferUsage usage);
    virtual ~GPUBuffer() = default;

    std::size_t getSize() const;
    virtual bool uploadData(StreamBuffer& buffer) = 0;

  protected:
    BufferType mType;
    BufferUsage mUsage;
    std::size_t mSize{0};
  };
}  // namespace GLaDOS

#endif
