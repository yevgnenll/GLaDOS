#include "ByteBuffer.h"

namespace GLaDOS {
    Logger* ByteBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("ByteBuffer");
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

    void ByteBuffer::setBufferData(void* data) {
        mBufferData = data;
    }

    void ByteBuffer::allocate() {
        if (mSize > 0) {
            mBufferData = MALLOC(mSize);
            mIsAllocated = true;
        } else {
            LOG_ERROR(logger, "ByteBuffer size is 0.");
        }
    }

}  // namespace GLaDOS