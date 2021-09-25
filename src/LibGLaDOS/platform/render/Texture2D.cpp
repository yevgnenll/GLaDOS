#include "Texture2D.h"

#include "memory/StreamBuffer.h"
#include "utils/Utility.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

namespace GLaDOS {
    Logger* Texture2D::logger = LoggerRegistry::getInstance().makeAndGetLogger("Texture2D");
    Texture2D::Texture2D(const std::string& name, PixelFormat format) : Texture{name, format} {
        mDimension = TextureDimension::Tex2D;
        mUsage = TextureUsage::ShaderRead;
    }

    Texture2D::~Texture2D() {
    }

    bool Texture2D::loadTextureFromFile() {
        int width;
        int height;
        int channels;
        std::string filename = mFileDirectory + mName;
        int result = stbi_info(filename.c_str(), &width, &height, &channels);
        if (result == 0) {
            LOG_ERROR(logger, "Failed to load texture {0}, reason: {1}", mName, stbi_failure_reason());
            return false;
        }

        int desiredChannel = 0;
        if (channels == 3) {  // metal does not support RGB format use RGBA format instead
            desiredChannel = 4;
        }
        uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, desiredChannel);

        if (data == nullptr) {
            LOG_ERROR(logger, "Failed to load texture {0}, reason: {1}", mName, stbi_failure_reason());
            return false;
        }

        mWidth = static_cast<uint32_t>(width);
        mHeight = static_cast<uint32_t>(height);
        if (desiredChannel == 0) {
            mChannels = static_cast<uint32_t>(channels);
        } else {
            mChannels = static_cast<uint32_t>(desiredChannel);
        }

        generateTexture(0, 0, data);
        stbi_image_free(data);

        LOG_TRACE(logger, "Texture load success: [name={0}, width={1}, height={2}, bpp={3}, useMipmap={4}, mipmapCount={5}]", mName, mWidth, mHeight, mChannels, mUseMipmap, mMipmapCount);

        return true;
    }

    bool Texture2D::loadTextureFromBuffer(StreamBuffer& buffer) {
        int width;
        int height;
        int channels;
        unsigned char* data = stbi_load_from_memory(static_cast<const stbi_uc*>(buffer.constPointer()),
                                                    static_cast<int>(buffer.size()), &width, &height, &channels, 0);

        if (data == nullptr) {
            LOG_ERROR(logger, "Failed to create texture from memory size {0}", buffer.size());
            return false;
        }

        mWidth = static_cast<uint32_t>(width);
        mHeight = static_cast<uint32_t>(height);
        mChannels = static_cast<uint32_t>(channels);
        generateTexture(0, 0, data);
        stbi_image_free(data);

        return true;
    }

    uint32_t Texture2D::calculateMipmapsCount(uint32_t w, uint32_t h) {
        return Math::floor(Math::log2(Math::max(w, h))) + 1;
    }

    bool Texture2D::generateMipmapsTexture(uint32_t x, uint32_t y, uint8_t* data) {
        if (mMipmapCount == 1) {
            return true;
        }

        uint32_t mipWidth = mWidth;
        uint32_t mipHeight = mHeight;
        for (uint32_t level = 1; level < mMipmapCount; level++) {
            mipWidth = (mipWidth >> 1);
            mipHeight = (mipHeight >> 1);
            uint8_t* mipData = static_cast<uint8_t*>(MALLOC(mipWidth * mipHeight * mChannels));
            if (stbir_resize_uint8(data, mWidth, mHeight, 0, mipData, mipWidth, mipHeight, 0, mChannels) == 0) {
                LOG_ERROR(logger, "Failed to create mipmap [name={0}, width={1}, height={2}, level={3}, bpp={4}]", mName, mipWidth, mipHeight, level, mChannels);
                return false;
            }
            replaceRegion(x, y, mipWidth, mipHeight, level, mipData);
            LOG_TRACE(logger, "Mipmap successfully created [name={0}, width={1}, height={2}, level={3}, bpp={4}]", mName, mipWidth, mipHeight, level, mChannels);
            FREE(mipData);
        }

        return true;
    }

    bool Texture2D::checkMipmapsUsable() const {
        return Math::isPowerOfTwo(mWidth) && Math::isPowerOfTwo(mHeight) && mUseMipmap;
    }
}  // namespace GLaDOS