#ifndef GLADOS_TEXTURE2D_H
#define GLADOS_TEXTURE2D_H

#include "Texture.h"

namespace GLaDOS {
  class Texture2D : public Texture {
  public:
    Texture2D(const std::string& name, TextureFormat format);
    ~Texture2D() override;

    bool loadTextureFromFile() override;
    bool loadTextureFromBuffer(StreamBuffer& buffer) override;
    virtual void replaceRegion(uint32_t x, uint32_t y, uint8_t* data) = 0;
  };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE2D_H
