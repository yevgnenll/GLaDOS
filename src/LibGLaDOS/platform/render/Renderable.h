#ifndef GLADOS_RENDERABLE_H
#define GLADOS_RENDERABLE_H

#include "utils/UniqueId.h"

namespace GLaDOS {
  class Material;
  class Mesh;
  class Renderable : public UniqueId {
    friend class Renderer;
    friend class MetalRenderer;

  public:
    Renderable();
    virtual ~Renderable();

    virtual void build() = 0;
    virtual void bindParams() = 0;  // called very frame in rendering loop

    Mesh* getMesh() const { return mMesh; }
    Material* getMaterial() const { return mMaterial; }

  protected:
    Mesh* mMesh{nullptr};
    Material* mMaterial{nullptr};
  };
}  // namespace GLaDOS

#endif
