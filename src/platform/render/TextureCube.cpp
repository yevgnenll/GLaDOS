#include "TextureCube.h"

namespace GLaDOS {
  TextureCube::TextureCube(const std::string& name, TextureFormat format) : Texture{name, format} {
    mDimension = TextureDimension::CubeMapTex;
  }

  TextureCube::~TextureCube() {
  }

  bool TextureCube::loadTextureFromFile() {
    return true;
  }

  bool TextureCube::loadTextureFromBuffer(StreamBuffer& buffer) {
    return true;
  }
}  // namespace GLaDOS