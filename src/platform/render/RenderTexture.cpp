#include "RenderTexture.h"

namespace GLaDOS {
  RenderTexture::RenderTexture(TextureFormat format) : Texture{"", format} {
  }

  RenderTexture::~RenderTexture() {
  }
}  // namespace GLaDOS