#include "Texture.h"

#include "RootDir.h"
#include "math/Color.h"
#include "platform/Platform.h"
#include "platform/render/RenderState.h"
#include "platform/render/Renderer.h"

namespace GLaDOS {
  Texture::Texture(const std::string& name, TextureFormat format) : Resource{ResourceType::Texture}, mFormat{format} {
    setResourceDir(RESOURCE_DIR);
    setName(name);
  }

  Texture::~Texture() {
    DELETE_T(mSamplerState, SamplerState);
    DELETE_T(mColorKey, Color);
  }

  SamplerState* Texture::samplerState() {
    if (mSamplerState == nullptr) {
      SamplerDescription defaultDesc{};
      setSamplerState(defaultDesc);
    }
    return mSamplerState;
  }

  void Texture::setSamplerState(const SamplerDescription& desc) {
    if (mSamplerState != nullptr) {
      DELETE_T(mSamplerState, SamplerState);
    }
    mSamplerState = Platform::getRenderer()->createSamplerState(desc);
  }

  void Texture::setColorKey(const Color& colorKey) {
    if (mColorKey != nullptr) {
      DELETE_T(mColorKey, Color);
    }

    mColorKey = NEW_T(Color{colorKey});
  }

  Color Texture::colorKey() const {
    return *mColorKey;
  }

  uint32_t Texture::getWidth() const {
    return mWidth;
  }

  uint32_t Texture::getHeight() const {
    return mHeight;
  }

  uint32_t Texture::getChannels() const {
    return mChannels;
  }

  bool Texture::isUseMipmap() const {
    return mUseMipmap;
  }

  uint32_t Texture::mipmapCount() const {
    return mMipmapCount;
  }

  TextureFormat Texture::getTextureFormat() const {
    return mFormat;
  }

  int Texture::mapChannelNumberFrom(TextureFormat format) {
    switch (format) {
      case TextureFormat::Red8:
        return 1;
      case TextureFormat::RG16:
        return 1;
      case TextureFormat::RGB24:  // metal doesn't support rgb format
      case TextureFormat::RGBA32:
        return 4;
      case TextureFormat::BGRA32:
        return 4;
      case TextureFormat::Alpha8:
        return 1;
      case TextureFormat::sRGB24:
        return 3;
      case TextureFormat::sRGBA32:
        return 4;
      default:
        LOG_WARN("Unknown texture channel number fallback to 0");
        break;
    }

    return 0;
  }
}  // namespace GLaDOS