#ifndef GLADOS_METALGPUBUFFER_H
#define GLADOS_METALGPUBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/MTLBuffer.h>

#include "platform/render/GPUBuffer.h"

namespace GLaDOS {
    class Logger;
    class MetalGPUBuffer : public GPUBuffer {
      public:
        MetalGPUBuffer(GPUBufferType type, GPUBufferUsage usage);
        ~MetalGPUBuffer() override;

        bool uploadData(void* data, std::size_t size) override;
        id<MTLBuffer> getMetalBuffer() const;

      private:
        void release();

        static Logger* logger;

        id<MTLBuffer> mMetalBuffer{nil};
    };
}  // namespace GLaDOS

#endif

#endif
