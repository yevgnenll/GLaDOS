#ifndef GLADOS_UPLOADBUFFER_H
#define GLADOS_UPLOADBUFFER_H

#include "memory/StreamBuffer.h"

namespace GLaDOS {
    class Logger;
    class UploadBuffer {
      public:
        virtual ~UploadBuffer();

        std::size_t size() const;
        std::size_t count() const;
        std::size_t stride() const;
        void* buffer();
        void setBufferData(void* data);

      protected:
        UploadBuffer() = default;
        void allocate();

        void* mBufferData{nullptr};
        std::size_t mSize{0};
        std::size_t mStride{0};
        bool mIsAllocated{false};

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  // GLADOS_UPLOADBUFFER_H
