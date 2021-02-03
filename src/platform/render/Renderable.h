#ifndef GLADOS_RENDERABLE_H
#define GLADOS_RENDERABLE_H

#include <cstdint>

namespace GLaDOS {
  using RenderableId = int32_t;
  class Material;
  class Mesh;
  class Renderable {
    friend class Renderer;

  public:
    static Renderable* create(Mesh* _mesh, Material* _material);
    virtual void release() = 0;
    RenderableId getId() const;

  protected:
    explicit Renderable(RenderableId _id);
    virtual ~Renderable();

  private:
    RenderableId mId;
    Mesh* mMesh{nullptr};
    Material* mMaterial{nullptr};
  };
}  // namespace GLaDOS

#endif
