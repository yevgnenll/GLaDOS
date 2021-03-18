#include "Texture2D.h"

#include "memory/StreamBuffer.h"
#include "utils/Utility.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace GLaDOS {
  Texture2D::Texture2D(const std::string& name, TextureFormat format) : Texture{name, format} {
    mDimension = TextureDimension::Tex2D;
    mUsage = TextureUsage::ShaderRead;
  }

  Texture2D::~Texture2D() {
  }

  bool Texture2D::loadTextureFromFile() {
    int width;
    int height;
    int channels;
    const char* filename = (mFileDirectory + mName).c_str();
    int result = stbi_info(filename, &width, &height, &channels);
    if (result == 0) {
      LOG_ERROR("Failed to load texture {0}", mName);
      return false;
    }

    int desiredChannel = 0;
    if (channels == 3) { // metal does not support RGB format use RGBA format instead
      desiredChannel = 4;
    }
    uint8_t* data = stbi_load(filename, &width, &height, &channels, desiredChannel);

    if (data == nullptr) {
      LOG_ERROR("Failed to load texture {0}", mName);
      return false;
    }

    LOG_TRACE("Texture load success: [name={0}, width={1}, height={2}, bpp={3}]", mName, width, height, channels);

    mWidth = static_cast<uint32_t>(width);
    mHeight = static_cast<uint32_t>(height);
    if (desiredChannel == 0) {
      mChannels = static_cast<uint32_t>(channels);
    } else {
      mChannels = static_cast<uint32_t>(desiredChannel);
    }

    replaceRegion(0, 0, data);
    stbi_image_free(data);

    return true;
  }

  bool Texture2D::loadTextureFromBuffer(StreamBuffer& buffer) {
    int width;
    int height;
    int channels;
    unsigned char* data = stbi_load_from_memory(static_cast<const stbi_uc*>(buffer.constPointer()),
                                                static_cast<int>(buffer.size()), &width, &height, &channels, 0);

    if (data == nullptr) {
      LOG_ERROR("Failed to create texture from memory size {0}", buffer.size());
      return false;
    }

    mWidth = static_cast<uint32_t>(width);
    mHeight = static_cast<uint32_t>(height);
    mChannels = static_cast<uint32_t>(channels);
    replaceRegion(0, 0, data);
    stbi_image_free(data);

    return true;
  }
}  // namespace GLaDOS