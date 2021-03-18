#include "MetalRenderTexture.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  MetalRenderTexture::MetalRenderTexture(TextureFormat format) : RenderTexture{format} {
  }

  MetalRenderTexture::~MetalRenderTexture() {
  }
}

#endif