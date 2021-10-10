#ifndef GLADOS_GPUBUFFER_H
#define GLADOS_GPUBUFFER_H

#include "memory/Blob.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class GPUBuffer {
      public:
        GPUBuffer(GPUBufferType type, GPUBufferUsage usage);
        virtual ~GPUBuffer() = default;

        std::size_t getSize() const;
        virtual bool uploadData(void* data, std::size_t size) = 0;

      protected:
        GPUBufferType mType;
        GPUBufferUsage mUsage;
        std::size_t mSize{0};
    };
}  // namespace GLaDOS

#endif
