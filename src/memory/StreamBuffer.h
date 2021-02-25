#ifndef GLADOS_STREAMBUFFER_H
#define GLADOS_STREAMBUFFER_H

#include <cstdint>

#include "utils/Utility.h"

namespace GLaDOS {
  class StreamBuffer {
  public:
    StreamBuffer() = default;
    virtual ~StreamBuffer() = default;

    StreamBuffer& operator<<(int8_t i);
    StreamBuffer& operator<<(int16_t i);
    StreamBuffer& operator<<(int32_t i);
    StreamBuffer& operator<<(int64_t i);
    StreamBuffer& operator<<(uint8_t i);
    StreamBuffer& operator<<(uint16_t i);
    StreamBuffer& operator<<(uint32_t i);
    StreamBuffer& operator<<(uint64_t i);
    StreamBuffer& operator<<(float i);
    StreamBuffer& operator<<(double i);

    void* offsetOf(std::size_t offset);
    void* pointer();
    std::size_t size() const;
    void resize(std::size_t n);
    void clear();
    bool isEmpty() const;

  private:
    void writeBytes(std::byte* bytes, unsigned int count);

    Vector<std::byte> mData;
  };
}  // namespace GLaDOS

#endif
