#include "Renderer.h"

namespace GLaDOS {
  Renderer::~Renderer() {
    for (auto it : mRenderable) {
      DELETE_T(it.second, Renderable);
    }
    mRenderable.clear();
  }

  Renderable* Renderer::getRenderable(RenderableId id) {
    auto it = mRenderable.find(id);
    if (it != mRenderable.end()) {
      return it->second;
    }

    return nullptr;
  }

  void Renderer::releaseRenderable(Renderable* renderable) {
    auto it = mRenderable.find(renderable->getId());
    if (it != mRenderable.end()) {
      mRenderable.erase(it);
      DELETE_T(it->second, Renderable);
    }
  }
}  // namespace GLaDOS