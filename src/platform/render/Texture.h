#ifndef GLADOS_TEXTURE_H
#define GLADOS_TEXTURE_H

#include "resource/Resource.h"
#include "math/Color.h"

namespace GLaDOS {
  class SamplerState;
  class Texture : public Resource {
  public:
    Texture();
    ~Texture();

    uint8_t* nativePointer();

  private:
    uint8_t* mNativeTexturePointer{nullptr};
    int mWidth{0};
    int mHeight{0};
    TextureFormat mFormat{TextureFormat::Unknown};
    TextureUsage mUsage{TextureUsage::Unknown};
    TextureDimension mDimension{TextureDimension::Unknown};
    SamplerState* mSamplerState{nullptr};
    bool mIsUseColorKey{false};
    Color mColorKey;
  };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE_H
