#ifndef GLADOS_UPLOADBUFFER_H
#define GLADOS_UPLOADBUFFER_H

#include "memory/Blob.h"

namespace GLaDOS {
    class Logger;
    class UploadBuffer {
      public:
        virtual ~UploadBuffer() = default;

        std::size_t size() const;
        std::size_t count() const;
        std::size_t stride() const;
        void* buffer();
        virtual void copyBufferData(void* data) = 0;

        UploadBuffer(const UploadBuffer& other) = delete;
        UploadBuffer& operator=(const UploadBuffer& other) = delete;

      protected:
        UploadBuffer() = default;

        Blob mBufferData;
        std::size_t mSize{0};
        std::size_t mStride{0};

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  // GLADOS_UPLOADBUFFER_H
