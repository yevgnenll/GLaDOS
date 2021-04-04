#include "ByteBuffer.h"

#include <cstring>

namespace GLaDOS {
  ByteBuffer::~ByteBuffer() {
    if (mIsAllocated) {
      FREE(mBufferData);
    }
  }

  std::size_t ByteBuffer::size() const {
    return mSize;
  }

  std::size_t ByteBuffer::count() const {
    return size() / stride();
  }

  std::size_t ByteBuffer::stride() const {
    return mStride;
  }

  void* ByteBuffer::buffer() {
    return mBufferData;
  }

  void ByteBuffer::uploadDataNoCopy(void* data) {
    mBufferData = data;
  }

  void ByteBuffer::allocate() {
    if (mSize > 0) {
      mBufferData = MALLOC(mSize);
      mIsAllocated = true;
    } else {
      LOG_ERROR("default", "ByteBuffer size is 0.");
    }
  }

}  // namespace GLaDOS