#include "Renderable.h"

#include "Renderer.h"

namespace GLaDOS {
  Renderable::Renderable(RenderableId _id) : mId{_id} {
  }

  Renderable::~Renderable() {
  }

  Renderable* Renderable::create(Mesh* _mesh, Material* _material) {
    // TODO
    return nullptr;
  }

  RenderableId Renderable::getId() const {
    return mId;
  }
}  // namespace GLaDOS