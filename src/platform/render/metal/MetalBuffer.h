#ifndef GLADOS_METALBUFFER_H
#define GLADOS_METALBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/MTLBuffer.h>

#include "platform/render/GPUBuffer.h"

namespace GLaDOS {
  class MetalBuffer : public GPUBuffer {
  public:
    MetalBuffer(BufferType type, BufferUsage usage);
    ~MetalBuffer() override;

    bool uploadData(StreamBuffer& buffer) override;
    id<MTLBuffer> getMetalBuffer() const;

  private:
    id<MTLBuffer> mMetalBuffer{nil};
  };
}  // namespace GLaDOS

#endif

#endif
