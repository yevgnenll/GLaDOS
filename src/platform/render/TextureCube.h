#ifndef GLADOS_TEXTURECUBE_H
#define GLADOS_TEXTURECUBE_H

#include "Texture.h"

namespace GLaDOS {
  class TextureCube : public Texture {
  public:
    TextureCube(const std::string& name, PixelFormat format);
    ~TextureCube() override;

    bool loadTextureFromFile(Vector<std::string>& names) override;
    bool loadTextureFromBuffer(const Vector<std::reference_wrapper<StreamBuffer>>& buffer) override;
    virtual void generateTexture(Vector<uint8_t*> data) = 0;
    virtual void replaceRegion(uint32_t size, uint32_t slice, uint32_t bytesPerRow, uint32_t bytesPerImage, uint8_t* data) = 0;
  };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURECUBE_H