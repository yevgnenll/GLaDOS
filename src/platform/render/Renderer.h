#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <vector>

#include "Buffer.h"
#include "utils/Enumeration.h"

namespace GameEngine {
  class Renderable;
  class ShaderProgram;
  class Renderer {
  public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool initialize() = 0;
    virtual void render() const = 0;

    virtual Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual ShaderProgram* createShaderProgram() = 0;

  private:
    std::vector<Renderable*> mRenderable;
  };
}  // namespace GameEngine

#endif
