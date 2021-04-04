#include "RenderTexture.h"

namespace GLaDOS {
  RenderTexture::RenderTexture(PixelFormat format) : Texture{"", format} {
  }

  RenderTexture::~RenderTexture() {
  }
}  // namespace GLaDOS