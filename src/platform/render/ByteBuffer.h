#ifndef GLADOS_BYTEBUFFER_H
#define GLADOS_BYTEBUFFER_H

#include "memory/StreamBuffer.h"

namespace GLaDOS {
  class ByteBuffer {
  public:
    virtual ~ByteBuffer() = default;

    std::size_t size() const;
    std::size_t count() const;
    std::size_t stride() const;
    StreamBuffer& buffer();
    void uploadData(StreamBuffer& buffer);
    void uploadData(const Vector<std::byte>& data);
    void uploadData(const std::byte* data);

  protected:
    ByteBuffer() = default;

    StreamBuffer mStreamBuffer;
    std::size_t mStride{0};
  };
}  // namespace GLaDOS

#endif  //GLADOS_BYTEBUFFER_H
