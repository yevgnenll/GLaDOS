#ifndef GLADOS_RENDERER_H
#define GLADOS_RENDERER_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class Mesh;
  class Material;
  class Buffer;
  class Renderable;
  class ShaderProgram;
  class FrameBuffer;
  class RenderBuffer;
  class VertexData;
  class IndexData;
  class Renderer {
  public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool initialize() = 0;
    virtual void render(Renderable* _renderable) = 0;

    virtual Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual ShaderProgram* createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) = 0;
    virtual Renderable* createRenderable(Mesh* mesh, Material* material) = 0;
    virtual Mesh* createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) = 0;
    virtual Mesh* createMesh(const std::string& meshPath, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) = 0;
    virtual FrameBuffer* createFrameBuffer() = 0;
    virtual RenderBuffer* createRenderBuffer() = 0;
  };
}  // namespace GLaDOS

#endif
