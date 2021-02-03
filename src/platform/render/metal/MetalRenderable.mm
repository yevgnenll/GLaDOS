#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderable.h"
#include "MetalRenderer.h"

namespace GLaDOS {
  GLaDOS::MetalRenderable::MetalRenderable(RenderableId _id) : Renderable{_id} {
  }

  GLaDOS::MetalRenderable::~MetalRenderable() {
  }

  void MetalRenderable::release() {
    MetalRenderer::getInstance()->releaseRenderable(this);
  }
}

#endif