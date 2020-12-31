#include "MetalRenderer.h"

#ifdef PLATFORM_MACOS

namespace GameEngine {
  MetalRenderer::MetalRenderer() {
  }

  MetalRenderer::~MetalRenderer() {
  }

  bool MetalRenderer::initialize() {
    return false;
  }

  void MetalRenderer::render(Renderable *renderable) const {

  }
}  // namespace GameEngine

#endif