#ifndef GLADOS_BYTECONTAINER_H
#define GLADOS_BYTECONTAINER_H

#include "memory/StreamBuffer.h"

namespace GLaDOS {
  class ByteContainer {
  public:
    ByteContainer() = default;
    virtual ~ByteContainer() = default;

    std::size_t size() const;
    std::size_t count() const;
    std::size_t stride() const;
    StreamBuffer& buffer();
    void uploadData(const Vector<std::byte>& data);
    void uploadData(const std::byte* data);
    void uploadData(const StreamBuffer& data);

  protected:
    StreamBuffer mStreamBuffer;
    std::size_t mStride{0};
  };
}  // namespace GLaDOS

#endif  //GLADOS_BYTECONTAINER_H
