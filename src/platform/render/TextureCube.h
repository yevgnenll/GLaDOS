#ifndef GLADOS_TEXTURECUBE_H
#define GLADOS_TEXTURECUBE_H

#include "Texture.h"

namespace GLaDOS {
  class TextureCube : public Texture {
  public:
    TextureCube(const std::string& name, TextureFormat format);
    ~TextureCube() override;

    bool loadTextureFromFile() override;
    bool loadTextureFromBuffer(StreamBuffer& buffer) override;

  private:
    CubeMapFace mCubeFace;
  };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURECUBE_H
