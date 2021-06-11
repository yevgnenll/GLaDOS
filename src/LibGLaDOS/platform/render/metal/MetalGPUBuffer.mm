#include "MetalGPUBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
  Logger* MetalGPUBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalGPUBuffer");
  MetalGPUBuffer::MetalGPUBuffer(BufferType type, BufferUsage usage) : GPUBuffer{type, usage} {}

  MetalGPUBuffer::~MetalGPUBuffer() {
    [mMetalBuffer release];
  }

  bool MetalGPUBuffer::uploadDataNoCopy(void* data, std::size_t size) {
    id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
    if (device == nil) {
      LOG_ERROR(logger, "Invalid Metal device state, upload GPUBuffer failed.");
      return false;
    }

    if (mMetalBuffer != nil) {
      [mMetalBuffer release];
    }

    std::size_t maxBufferLength = [device maxBufferLength];
    if (size > maxBufferLength) {
      LOG_ERROR(logger, "Must not exceeded Metal maximum buffer length({0}), but given size {1}", maxBufferLength, size);
      return false;
    }

    mSize = size;

    // https://developer.apple.com/documentation/metal/setting_resource_storage_modes/choosing_a_resource_storage_mode_in_macos?language=objc
    switch (mUsage) {
      case BufferUsage::Private:
        mMetalBuffer = [device newBufferWithBytes:data length:mSize options:MTLResourceStorageModePrivate];
        break;
      case BufferUsage::Synchronized:
        break;
      case BufferUsage::Shared:
        break;
    }

    //    id<MTLBuffer> buffer_with_host_mem = [device newBufferWithBytes:data length:size options:(MTLResourceStorageModeShared | MTLResourceCPUCacheModeWriteCombined)];
    //    id <MTLCommandBuffer> cmd_buffer = [[device newCommandQueue] commandBuffer];
    //    id <MTLBlitCommandEncoder> blit_encoder = [cmd_buffer blitCommandEncoder];
    //    [blit_encoder copyFromBuffer:buffer_with_host_mem
    //                    sourceOffset:0
    //                        toBuffer:mMetalBuffer
    //               destinationOffset:0
    //                            size:size];
    //    [blit_encoder endEncoding];
    //    [cmd_buffer commit];
    //    [cmd_buffer waitUntilCompleted];

    //    mMetalBuffer = [device newBufferWithBytes:data length:mSize options:MTLResourceCPUCacheModeDefaultCache];
    //    mMetalBuffer = [device newBufferWithBytesNoCopy:data length:alignment(mSize, _gpu_mem_alignment) options:MTLResourceCPUCacheModeDefaultCache deallocator:^(void* pointer, NSUInteger length) {
    //      align_free(pointer);
    //    }];
    if (mMetalBuffer == nil) {
      LOG_ERROR(logger, "Metal buffer initialization failed.");
      return false;
    }

    return true;
  }

  id<MTLBuffer> MetalGPUBuffer::getMetalBuffer() const {
    return mMetalBuffer;
  }
}

#endif
