#ifndef GLADOS_RENDERER_H
#define GLADOS_RENDERER_H

#include "Buffer.h"
#include "Renderable.h"
#include "utils/Enumeration.h"
#include "utils/Utility.h"

namespace GLaDOS {
  class Renderable;
  class ShaderProgram;
  class Renderer {
  public:
    Renderer() = default;
    virtual ~Renderer();

    virtual bool initialize() = 0;
    virtual void render(Renderable* renderable) = 0;

    virtual Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual ShaderProgram* createShaderProgram() = 0;
    virtual Renderable* createRenderable() = 0;
    Renderable* getRenderable(RenderableId id);
    void releaseRenderable(Renderable* renderable);

  protected:
    Map<RenderableId, Renderable*> mRenderable;
  };
}  // namespace GLaDOS

#endif
