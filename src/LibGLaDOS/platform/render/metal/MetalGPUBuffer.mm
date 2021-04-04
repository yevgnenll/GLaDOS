#include "MetalGPUBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
  MetalGPUBuffer::MetalGPUBuffer(BufferType type, BufferUsage usage) : GPUBuffer{type, usage} {}

  MetalGPUBuffer::~MetalGPUBuffer() {
    [mMetalBuffer release];
  }

  bool MetalGPUBuffer::uploadDataNoCopy(void* data, std::size_t size) {
    id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
    if (device == nil) {
      LOG_ERROR("default", "Invalid Metal device state, upload GPUBuffer failed.");
      return false;
    }

    if (mMetalBuffer != nil) {
      [mMetalBuffer release];
    }

    mSize = size;
    // https://developer.apple.com/documentation/metal/setting_resource_storage_modes/choosing_a_resource_storage_mode_in_macos?language=objc
    mMetalBuffer = [device newBufferWithBytes:data length:mSize options:MTLResourceCPUCacheModeDefaultCache];
    if (mMetalBuffer == nil) {
      LOG_ERROR("default", "Metal buffer initialization failed.");
      return false;
    }

    return true;
  }

  id<MTLBuffer> MetalGPUBuffer::getMetalBuffer() const {
    return mMetalBuffer;
  }
}

#endif
