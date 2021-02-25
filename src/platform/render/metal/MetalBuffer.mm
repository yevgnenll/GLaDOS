#include "MetalBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
  MetalBuffer::MetalBuffer(BufferType type, BufferUsage usage) : Buffer{type, usage} {}

  bool MetalBuffer::uploadData(StreamBuffer& buffer) {
    if (mMetalBuffer != nullptr) {
      [mMetalBuffer release];
    }

    mSize = buffer.size();

    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nullptr) {
      LOG_ERROR("Invalid Metal device state");
      return false;
    }

    mMetalBuffer = [device newBufferWithBytes:buffer.pointer() length:mSize options:MTLResourceOptionCPUCacheModeDefault];
    return true;
  }

  id<MTLBuffer> MetalBuffer::getMetalBuffer() const {
    return mMetalBuffer;
  }
}

#endif