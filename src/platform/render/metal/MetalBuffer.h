#ifndef GAMEENGINE_METALBUFFER_H
#define GAMEENGINE_METALBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/MTLBuffer.h>

#include "platform/render/Buffer.h"

namespace GameEngine {
  class MetalBuffer : public Buffer {
  public:
    MetalBuffer(BufferType type, BufferUsage usage, StreamBuffer& buffer);
    ~MetalBuffer() override = default;

    bool uploadData(StreamBuffer& buffer) override;
    id<MTLBuffer> getMetalBuffer() const;

  private:
    id<MTLBuffer> mMetalBuffer{nullptr};
  };
}  // namespace GameEngine

#endif

#endif
