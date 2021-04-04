#ifndef GLADOS_METALGPUBUFFER_H
#define GLADOS_METALGPUBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/MTLBuffer.h>

#include "platform/render/GPUBuffer.h"

namespace GLaDOS {
  class MetalGPUBuffer : public GPUBuffer {
  public:
    MetalGPUBuffer(BufferType type, BufferUsage usage);
    ~MetalGPUBuffer() override;

    bool uploadDataNoCopy(void* data, std::size_t size) override;
    id<MTLBuffer> getMetalBuffer() const;

  private:
    id<MTLBuffer> mMetalBuffer{nil};
  };
}  // namespace GLaDOS

#endif

#endif
