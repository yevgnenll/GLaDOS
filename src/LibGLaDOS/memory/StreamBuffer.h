#ifndef GLADOS_STREAMBUFFER_H
#define GLADOS_STREAMBUFFER_H

#include "math/Mat4.hpp"
#include "utils/Utility.h"

namespace GLaDOS {
  class Logger;
  class Vec2;
  class Vec3;
  class Vec4;
  class Quat;
  class Color;
  class StreamBuffer {
  public:
    StreamBuffer() = default;
    StreamBuffer(std::size_t size, void* data);
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
    StreamBuffer& operator<<(const Vec2& i);
    StreamBuffer& operator<<(const Vec3& i);
    StreamBuffer& operator<<(const Vec4& i);
    StreamBuffer& operator<<(const Quat& i);
    StreamBuffer& operator<<(const Color& i);
    template <typename T>
    StreamBuffer& operator<<(const Mat4<T>& i);

    void uploadData(StreamBuffer& buffer);
    void uploadData(const Vector<std::byte>& data);
    void uploadData(const std::byte* data, std::size_t size);

    void* offsetOf(std::size_t offset);
    void* pointer();
    const void* constPointer() const;
    std::size_t size() const;
    void resize(std::size_t n);
    void clear();
    bool isEmpty() const;

  protected:
    void throwIfOverflow(std::size_t size) const;

  private:
    void writeBytes(std::byte* bytes, unsigned int count);

    static Logger* logger;
    Vector<std::byte> mData;
  };

  template <typename T>
  StreamBuffer& StreamBuffer::operator<<(const Mat4<T>& i) {
    writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(i.pointer())), sizeof(T) * i.size());
    return *this;
  }
}  // namespace GLaDOS

#endif
