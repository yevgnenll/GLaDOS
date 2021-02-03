#ifndef GLADOS_GLRENDERER_H
#define GLADOS_GLRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_LINUX

#include "platform/render/Renderer.h"

namespace GLaDOS {
  class GLRenderer : public Renderer {
  public:
    GLRenderer();
    virtual ~GLRenderer();

    bool initialize() override;
    void render() const override;

    Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    ShaderProgram* createShaderProgram() override;

  private:
  };
}  // namespace GLaDOS

#endif

#endif