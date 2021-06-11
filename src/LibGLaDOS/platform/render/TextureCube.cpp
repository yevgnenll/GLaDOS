#include "TextureCube.h"

#include <stb_image.h>

#include <algorithm>

namespace GLaDOS {
  Logger* TextureCube::logger = LoggerRegistry::getInstance().makeAndGetLogger("TextureCube");
  TextureCube::TextureCube(const std::string& name, PixelFormat format) : Texture{name, format} {
    mDimension = TextureDimension::CubeMapTex;
    mUsage = TextureUsage::ShaderRead;
  }

  TextureCube::~TextureCube() {
  }

  bool TextureCube::loadTextureFromFile(Vector<std::string>& names) {
    if (names.empty() || names.size() != static_cast<uint32_t>(CubeMapFace::TheNumberOfFace)) {
      LOG_ERROR(logger, "Cubemap can only be created from exactly six images");
      return false;
    }

    struct InternalTextureData {
      uint32_t width, height;
      uint32_t channels;
      uint8_t* data;
      const char* name;
    };

    Vector<InternalTextureData> images;
    for (std::string const& name : names) {
      int width;
      int height;
      int channels;
      std::string filename{mFileDirectory + name};
      int result = stbi_info(filename.c_str(), &width, &height, &channels);
      if (result == 0) {
        LOG_ERROR(logger, "Failed to get info texture {0}, reason: {1}", filename, stbi_failure_reason());
        return false;
      }
      if (width != height) {
        LOG_ERROR(logger, "Failed to load texture {0}: Cubemap texture must be uniformly-sized.", filename);
        return false;
      }
      int desiredChannel = 0;
      if (channels == 3) {  // metal does not support RGB format use RGBA format instead
        desiredChannel = 4;
      }
      uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, desiredChannel);

      if (data == nullptr) {
        LOG_ERROR(logger, "Failed to load texture {0}, reason: {1}", filename, stbi_failure_reason());
        return false;
      }

      InternalTextureData textureData;
      textureData.width = static_cast<uint32_t>(width);
      textureData.height = static_cast<uint32_t>(height);
      textureData.channels = (desiredChannel == 0) ? static_cast<uint32_t>(channels) : static_cast<uint32_t>(desiredChannel);
      textureData.data = data;
      textureData.name = name.c_str();
      images.emplace_back(textureData);
    }

    bool isSame = std::all_of(images.begin(), images.end(), [&images](const InternalTextureData& it) {
      return it.width == images[0].width && it.channels == images[0].channels;
    });
    if (!isSame) {
      LOG_ERROR(logger, "All cubemap texture must be uniformly-sized and same channel bits.");
      for (auto& image : images) {
        stbi_image_free(image.data);
      }
      return false;
    }

    mWidth = images[0].width;
    mHeight = images[0].height;
    mChannels = images[0].channels;
    mUseMipmap = false;  // cubemap does not support mipmap generation.
    mMipmapCount = 1;

    Vector<uint8_t*> textureParameter;
    std::transform(images.begin(), images.end(), std::back_inserter(textureParameter), [](const InternalTextureData& it) { return it.data; });
    generateTexture(textureParameter);

    for (auto& image : images) {
      LOG_TRACE(logger, "CubeTexture load success: [name={0}, width={1}, height={2}, bpp={3}]", image.name, image.width, image.height, image.channels);
      stbi_image_free(image.data);
    }

    return true;
  }

  bool TextureCube::loadTextureFromBuffer(const Vector<std::reference_wrapper<StreamBuffer>>& buffer) {
    return true;
  }
}  // namespace GLaDOS