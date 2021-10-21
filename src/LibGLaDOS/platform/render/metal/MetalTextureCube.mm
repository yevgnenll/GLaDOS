#include "MetalTextureCube.h"

#ifdef PLATFORM_MACOS

#import "MetalRenderState.h"
#include "MetalTypes.h"

namespace GLaDOS {
    Logger* MetalTextureCube::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalTextureCube");

    MetalTextureCube::MetalTextureCube(const std::string& name, PixelFormat format) : TextureCube{name, format} {
    }

    id<MTLSamplerState> MetalTextureCube::metalSamplerState() {
        return static_cast<MetalSamplerState*>(samplerState())->getMetalSamplerState();
    }

    void MetalTextureCube::generateTexture(Vector<uint8_t*> data) {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state, texture creation failed.");
            return;
        }

        if (data.empty() || data.size() != static_cast<uint32_t>(CubeMapFace::TheNumberOfFace)) {
            LOG_ERROR(logger, "Cubemap can only be created from exactly six images");
            return;
        }

        release();

        mTextureDescriptor = [MTLTextureDescriptor new];
        mTextureDescriptor.pixelFormat = MetalTypes::pixelFormatToMetal(mFormat);
        mTextureDescriptor.width = mWidth;
        mTextureDescriptor.height = mHeight;
        mTextureDescriptor.usage = MetalTypes::textureUsageToMetal(mUsage);
        mTextureDescriptor.textureType = MTLTextureTypeCube;
        mTextureDescriptor.mipmapLevelCount = 1;
        mTexture = [device newTextureWithDescriptor:mTextureDescriptor];

        if (mTexture == nil) {
            LOG_ERROR(logger, "Failed to create Texture: {0}", mName);
            return;
        }

        const uint32_t bytesPerRow = mChannels * mWidth;
        const uint32_t bytesPerImage = bytesPerRow * mWidth;

        for (uint32_t slice = 0; slice < static_cast<uint32_t>(CubeMapFace::TheNumberOfFace); slice++) {
            replaceRegion(mWidth, slice, bytesPerRow, bytesPerImage, data[slice]);
        }
    }

    void MetalTextureCube::replaceRegion(uint32_t size, uint32_t slice, uint32_t bytesPerRow, uint32_t bytesPerImage, uint8_t* data) {
        MTLRegion region = {{0, 0, 0}, {size, size, 1}};
        [mTexture replaceRegion:region mipmapLevel:0 slice:slice withBytes:data bytesPerRow:bytesPerRow bytesPerImage:bytesPerImage];
    }
}

#endif