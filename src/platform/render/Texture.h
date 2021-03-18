#ifndef GLADOS_TEXTURE_H
#define GLADOS_TEXTURE_H

#include "resource/Resource.h"

namespace GLaDOS {
  class Color;
  class SamplerState;
  class SamplerDescription;
  class StreamBuffer;
  class Texture : public Resource {
  public:
    Texture(const std::string& name, TextureFormat format);
    ~Texture() override;

    SamplerState* samplerState();
    void setSamplerState(const SamplerDescription& desc);
    void setColorKey(const Color& colorKey);
    Color colorKey() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getChannels() const;
    bool isUseMipmap() const;
    uint32_t mipmapCount() const;
    TextureFormat getTextureFormat() const;

    virtual bool loadTextureFromFile() = 0;
    virtual bool loadTextureFromBuffer(StreamBuffer& buffer) = 0;

  protected:
    static int mapChannelNumberFrom(TextureFormat format);

    uint32_t mWidth{0};
    uint32_t mHeight{0};
    uint32_t mChannels{0};
    bool mUseMipmap{true};
    uint32_t mMipmapCount{1};
    TextureFormat mFormat{TextureFormat::Unknown};
    TextureUsage mUsage{TextureUsage::Unknown};
    TextureDimension mDimension{TextureDimension::Unknown};
    SamplerState* mSamplerState{nullptr};
    Color* mColorKey{nullptr};
  };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE_H
