#include "MeshRenderer.h"
#include "platform/render/Renderer.h"
#include "platform/render/Renderable.h"

namespace GLaDOS {
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