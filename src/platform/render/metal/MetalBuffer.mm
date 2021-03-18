#include "MetalBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
  MetalBuffer::MetalBuffer(BufferType type, BufferUsage usage) : Buffer{type, usage} {}

  MetalBuffer::~MetalBuffer() {
    [mMetalBuffer release];
  }

  bool MetalBuffer::uploadData(StreamBuffer& buffer) {
    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nil) {
      LOG_ERROR("Invalid Metal device state, upload Buffer failed.");
      return false;
    }

    if (mMetalBuffer != nil) {
      [mMetalBuffer release];
    }

    mSize = buffer.size();
    mMetalBuffer = [device newBufferWithBytes:buffer.pointer() length:mSize options:MTLResourceOptionCPUCacheModeDefault];
    if (mMetalBuffer == nil) {
      LOG_ERROR("Metal buffer initialization failed.");
      return false;
    }

    return true;
  }

  id<MTLBuffer> MetalBuffer::getMetalBuffer() const {
    return mMetalBuffer;
  }
}

#endif