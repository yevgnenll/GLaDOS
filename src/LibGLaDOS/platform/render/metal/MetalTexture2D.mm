#include "MetalTexture2D.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderState.h"
#include "MetalTypes.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
        mTextureDescriptor.pixelFormat = MetalTypes::pixelFormatToMetal(mFormat);
        mTextureDescriptor.width = mWidth;
        mTextureDescriptor.height = mHeight;
        mTextureDescriptor.usage = MetalTypes::textureUsageToMetal(mUsage);
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

    Blob MetalTexture2D::encodeToPNG() const {
        const uint32_t bytesPerPixel = Texture::mapChannelNumberFrom(mFormat);

        // The total number of bytes of the texture
        const uint32_t imageByteCount = mWidth * mHeight * bytesPerPixel;

        // The number of bytes for each image row
        uint32_t bytesPerRow = mWidth * bytesPerPixel;
        void* sourceBuffer = MALLOC(sizeof(unsigned char) * imageByteCount);

        // Gets the bytes from the texture
        MTLRegion region = MTLRegionMake2D(0, 0, mWidth, mHeight);
        [mTexture getBytes:sourceBuffer bytesPerRow:bytesPerRow fromRegion:region mipmapLevel:0];

        int len;
        unsigned char* png = stbi_write_png_to_mem(
            reinterpret_cast<unsigned char *>(sourceBuffer), bytesPerRow, mWidth, mHeight, bytesPerPixel, &len);
        FREE(sourceBuffer);
        if (png == nullptr) {
            LOG_ERROR(logger, "error to encode texture to png");
            return Blob{};
        }

        return Blob{reinterpret_cast<std::byte*>(png), static_cast<std::size_t>(len)};
    }

    Blob MetalTexture2D::encodeToJPG() const {
        // https://github.com/nothings/stb/issues/1132
        return Blob(); // TODO: stbi_write_jpg_to_mem 에 대응되는 함수가 없음
    }

    Blob MetalTexture2D::encodeToBMP() const {
        return Blob(); // TODO: stbi_write_bmp_to_mem 에 대응되는 함수가 없음
    }

    Blob MetalTexture2D::encodeToTGA() const {
        return Blob(); // TODO: stbi_write_tga_to_mem 에 대응되는 함수가 없음
    }
}

#endif