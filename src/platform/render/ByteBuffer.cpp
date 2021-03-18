#include "ByteBuffer.h"

#include <cstring>

namespace GLaDOS {
  std::size_t ByteBuffer::size() const {
    return mStreamBuffer.size();
  }

  std::size_t ByteBuffer::count() const {
    return size() / stride();
  }

  std::size_t ByteBuffer::stride() const {
    return mStride;
  }

  StreamBuffer& ByteBuffer::buffer() {
    return mStreamBuffer;
  }

  void ByteBuffer::uploadData(const Vector<std::byte>& data) {
    std::memcpy(mStreamBuffer.pointer(), data.data(), size());
  }

  void ByteBuffer::uploadData(const std::byte* data) {
    std::memcpy(mStreamBuffer.pointer(), data, size());
  }
}  // namespace GLaDOS