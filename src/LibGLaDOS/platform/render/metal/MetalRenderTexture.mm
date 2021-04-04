#include "MetalRenderTexture.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  MetalRenderTexture::MetalRenderTexture(PixelFormat format) : RenderTexture{format} {
  }

  MetalRenderTexture::~MetalRenderTexture() {
  }
}

#endif