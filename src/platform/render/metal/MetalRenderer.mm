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

  void MetalRenderer::render() const {

  }
}  // namespace GameEngine

#endif