#include "UploadBuffer.h"

namespace GLaDOS {
    Logger* UploadBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("UploadBuffer");
    UploadBuffer::~UploadBuffer() {
        if (mIsAllocated) {
            FREE(mBufferData);
        }
    }

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
        return mBufferData;
    }

    void UploadBuffer::setBufferData(void* data) {
        mBufferData = data;
    }

    void UploadBuffer::allocate() {
        if (mSize > 0) {
            mBufferData = MALLOC(mSize);
            mIsAllocated = true;
        } else {
            LOG_ERROR(logger, "UploadBuffer size is 0.");
        }
    }

}  // namespace GLaDOS