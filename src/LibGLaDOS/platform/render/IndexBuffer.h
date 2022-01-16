#ifndef GLADOS_INDEXBUFFER_H
#define GLADOS_INDEXBUFFER_H

#include "UploadBuffer.h"

namespace GLaDOS {
    class IndexBuffer : public UploadBuffer {
      public:
        IndexBuffer(std::size_t stride, std::size_t count);
        ~IndexBuffer() override = default;

        void copyBufferData(void *data) override;
    };
}  // namespace GLaDOS

#endif  // GLADOS_INDEXBUFFER_H
