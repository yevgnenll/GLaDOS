#include "MetalTexture2D.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderState.h"
#include "math/Math.h"

namespace GLaDOS {
    Logger* MetalTexture2D::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalTexture2D");
    MetalTexture2D::MetalTexture2D(const std::string& name, PixelFormat format) : Texture2D{name, format} {
    }

    MetalTexture2D::~MetalTexture2D() {
    }

    void MetalTexture2D::generateTexture(uint32_t x, uint32_t y, uint8_t* data) {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state, texture creation failed.");
            return;
        }

        release();

        mTextureDescriptor = [MTLTextureDescriptor new];
        mTextureDescriptor.pixelFormat = MetalTextureBase::mapMetalPixelFormatFrom(mFormat);
        mTextureDescriptor.width = mWidth;
        mTextureDescriptor.height = mHeight;
        mTextureDescriptor.usage = MetalTextureBase::mapMetalTextureUsageFrom(mUsage);
        mTextureDescriptor.textureType = MTLTextureType2D;
        mTextureDescriptor.mipmapLevelCount = checkMipmapsUsable() ? calculateMipmapsCount(mWidth, mHeight) : 1; // The default value is 1
        mTexture = [device newTextureWithDescriptor:mTextureDescriptor];
        mMipmapCount = static_cast<uint32_t>(mTextureDescriptor.mipmapLevelCount);

        if (mTexture == nil) {
            LOG_ERROR(logger, "Failed to create Texture: {0}", mName);
            return;
        }

        if (!generateMipmapsTexture(x, y, data)) {
            return;
        }

        replaceRegion(x, y, mWidth, mHeight, 0, data);
    }

    void MetalTexture2D::replaceRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t level, uint8_t* data) {
        MTLRegion region = {{x, y, 0}, {w, h, 1}};
        uint32_t bytesPerRow = mChannels * w;
        [mTexture replaceRegion:region mipmapLevel:level withBytes:data bytesPerRow:bytesPerRow];
    }

    id<MTLSamplerState> MetalTexture2D::metalSamplerState() {
        return static_cast<MetalSamplerState*>(samplerState())->getMetalSamplerState();
    }
}

#endif