#include "ByteContainer.h"

namespace GLaDOS {
  std::size_t ByteContainer::size() const {
    return mStreamBuffer.size();
  }

  std::size_t ByteContainer::count() const {
    return size() / stride();
  }

  std::size_t ByteContainer::stride() const {
    return mStride;
  }

  StreamBuffer& ByteContainer::buffer() {
    return mStreamBuffer;
  }

  void ByteContainer::uploadData(const Vector<std::byte>& data) {
    std::memcpy(mStreamBuffer.pointer(), data.data(), size());
  }

  void ByteContainer::uploadData(const std::byte* data) {
    std::memcpy(mStreamBuffer.pointer(), data, size());
  }

  void ByteContainer::uploadData(const StreamBuffer& data) {
    mStreamBuffer = data;
  }
}  // namespace GLaDOS