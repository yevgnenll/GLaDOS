#include "Texture.h"

namespace GLaDOS {
  Texture::Texture() : Resource{ResourceType::Texture} {

  }

  Texture::~Texture() {

  }

  uint8_t* Texture::nativePointer() {
    return mNativeTexturePointer;
  }
}