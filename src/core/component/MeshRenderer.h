#ifndef GLADOS_MESHRENDERER_H
#define GLADOS_MESHRENDERER_H

#include "core/Component.h"

namespace GLaDOS {
  class Renderable;
  class MeshRenderer : public Component {
  public:
    MeshRenderer() = default;
    ~MeshRenderer() override;

    void setRenderable(Renderable* renderable);

  private:
    virtual void update(real deltaTime);
    virtual void render();

    Renderable* mRenderable;
  };
}  // namespace GLaDOS

#endif  //GLADOS_MESHRENDERER_H