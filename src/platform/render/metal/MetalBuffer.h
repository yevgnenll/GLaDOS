#ifndef GLADOS_METALBUFFER_H
#define GLADOS_METALBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/MTLBuffer.h>

#include "platform/render/Buffer.h"

namespace GLaDOS {
  class MetalBuffer : public Buffer {
  public:
    MetalBuffer(BufferType type, BufferUsage usage);
    ~MetalBuffer() override = default;

    bool uploadData(StreamBuffer& buffer) override;
    id<MTLBuffer> getMetalBuffer() const;

  private:
    id<MTLBuffer> mMetalBuffer{nil};
  };
}  // namespace GLaDOS

#endif

#endif
