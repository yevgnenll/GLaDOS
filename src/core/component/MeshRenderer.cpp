#include "MeshRenderer.h"

#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"

namespace GLaDOS {
  MeshRenderer::MeshRenderer() : Component{"MeshRenderer"} {
  }

  MeshRenderer::~MeshRenderer() {
    DELETE_T(mRenderable, Renderable);
  }

  void MeshRenderer::setRenderable(Renderable* renderable) {
    mRenderable = renderable;
  }

  void MeshRenderer::update(real deltaTime) {
  }

  void MeshRenderer::render() {
    Platform::getRenderer()->render(mRenderable);
  }
}  // namespace GLaDOS