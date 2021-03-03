#ifndef GLADOS_MESHRENDERER_H
#define GLADOS_MESHRENDERER_H

#include "core/Component.h"

namespace GLaDOS {
  class Renderable;
  class MeshRenderer : public Component {
  public:
    MeshRenderer();
    ~MeshRenderer() override;

    void setRenderable(Renderable* renderable);

  private:
    void update(real deltaTime) override;
    void render() override;

    Renderable* mRenderable;
  };
}  // namespace GLaDOS

#endif  //GLADOS_MESHRENDERER_H