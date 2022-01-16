#include "UploadBuffer.h"

namespace GLaDOS {
    Logger* UploadBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("UploadBuffer");
    std::size_t UploadBuffer::size() const {
        return mSize;
    }

    std::size_t UploadBuffer::count() const {
        return size() / stride();
    }

    std::size_t UploadBuffer::stride() const {
        return mStride;
    }

    void* UploadBuffer::buffer() {
        return mBufferData.pointer();
    }
}  // namespace GLaDOS