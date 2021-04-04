#ifndef GLADOS_BYTEBUFFER_H
#define GLADOS_BYTEBUFFER_H

#include "memory/StreamBuffer.h"

namespace GLaDOS {
  class ByteBuffer {
  public:
    virtual ~ByteBuffer();

    std::size_t size() const;
    std::size_t count() const;
    std::size_t stride() const;
    void* buffer();
    void uploadDataNoCopy(void* data);

  protected:
    ByteBuffer() = default;
    void allocate();

    void* mBufferData;
    std::size_t mSize{0};
    std::size_t mStride{0};
    bool mIsAllocated{false};
  };
}  // namespace GLaDOS

#endif  //GLADOS_BYTEBUFFER_H
