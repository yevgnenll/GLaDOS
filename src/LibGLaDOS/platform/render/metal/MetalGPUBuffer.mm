#include "MetalGPUBuffer.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
    Logger* MetalGPUBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalGPUBuffer");
    MetalGPUBuffer::MetalGPUBuffer(GPUBufferType type, GPUBufferUsage usage) : GPUBuffer{type, usage} {}

    MetalGPUBuffer::~MetalGPUBuffer() {
        release();
    }

    bool MetalGPUBuffer::uploadData(void* data, std::size_t size) {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state, upload GPUBuffer failed.");
            return false;
        }

        release();

        std::size_t maxBufferLength = [device maxBufferLength];
        if (size > maxBufferLength) {
            LOG_ERROR(logger, "Must not exceeded Metal maximum buffer length({0}), but given size is {1}", maxBufferLength, size);
            return false;
        }

        mSize = size;
        // https://developer.apple.com/documentation/metal/setting_resource_storage_modes/choosing_a_resource_storage_mode_in_macos?language=objc
        mMetalBuffer = [device newBufferWithBytes:data length:mSize options:MTLResourceCPUCacheModeDefaultCache];

        if (mMetalBuffer == nil) {
            LOG_ERROR(logger, "Metal buffer initialization failed.");
            return false;
        }

        return true;
    }

    id<MTLBuffer> MetalGPUBuffer::getMetalBuffer() const {
        return mMetalBuffer;
    }

    void MetalGPUBuffer::release() {
        if (mMetalBuffer != nil) {
            [mMetalBuffer release];
        }
    }
}

#endif
