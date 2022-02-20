#include "MetalRenderTexture.h"

#ifdef PLATFORM_MACOS

#include "MetalTypes.h"
#include "MetalRenderState.h"

namespace GLaDOS {
    Logger* MetalRenderTexture::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalRenderTexture");

    MetalRenderTexture::MetalRenderTexture(PixelFormat format) : RenderTexture{format} {
    }

    MetalRenderTexture::~MetalRenderTexture() {
    }

    bool MetalRenderTexture::generateTexture() {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state, texture creation failed.");
            return false;
        }

        release();

        mTextureDescriptor = [MTLTextureDescriptor new];
        mTextureDescriptor.pixelFormat = MetalTypes::pixelFormatToMetal(mFormat);
        mTextureDescriptor.width = mWidth;
        mTextureDescriptor.height = mHeight;
        mTextureDescriptor.usage = MetalTypes::textureUsageToMetal(mUsage);
        mTextureDescriptor.textureType = MTLTextureType2D;
        mTextureDescriptor.mipmapLevelCount = 1; // RT does not create mipmap.
        mTexture = [device newTextureWithDescriptor:mTextureDescriptor];
        mMipmapCount = static_cast<uint32_t>(mTextureDescriptor.mipmapLevelCount);

        if (mTexture == nil) {
            LOG_ERROR(logger, "Failed to create Texture: {0}", mName);
            return false;
        }

        return true;
    }

    id<MTLSamplerState> MetalRenderTexture::metalSamplerState() {
        return static_cast<MetalSamplerState*>(samplerState())->getMetalSamplerState();
    }
}

#endif