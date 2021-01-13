#include "MetalBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GameEngine {
  MetalBuffer::MetalBuffer(BufferType type, BufferUsage usage, StreamBuffer& buffer) : Buffer{type, usage, buffer} {
    uploadData(buffer);
  }

  bool MetalBuffer::uploadData(StreamBuffer& buffer) {
    if (mMetalBuffer != nullptr) {
      [mMetalBuffer release];
    }

    id<MTLDevice> device = MetalRenderer::getInstance()->getMetalDevice();
    if (device != nullptr) {
      mMetalBuffer = [device newBufferWithBytes:buffer.pointer() length:buffer.size() options:MTLResourceOptionCPUCacheModeDefault];
      return true;
    }

    return false;
  }

  id<MTLBuffer> MetalBuffer::getMetalBuffer() const {
    return mMetalBuffer;
  }
}

#endif