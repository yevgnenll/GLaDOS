#ifndef GLADOS_RENDERER_H
#define GLADOS_RENDERER_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class Mesh;
  class Material;
  class GPUBuffer;
  class Renderable;
  class ShaderProgram;
  class FrameBuffer;
  class RenderBuffer;
  class VertexData;
  class IndexData;
  class DepthStencilState;
  class DepthStencilDescription;
  class SamplerState;
  class SamplerDescription;
  class Texture2D;
  class Texture3D;
  class TextureCube;
  class RenderTexture;
  class Renderer {
  public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool initialize() = 0;
    virtual void render(Renderable* _renderable) = 0;

    virtual GPUBuffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual GPUBuffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) = 0;
    virtual ShaderProgram* createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const VertexData* vertexData) = 0;
    virtual Renderable* createRenderable(Mesh* mesh, Material* material) = 0;
    virtual Mesh* createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) = 0;
    virtual Mesh* createMesh(const std::string& meshPath, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) = 0;
    virtual FrameBuffer* createFrameBuffer() = 0;
    virtual RenderBuffer* createRenderBuffer() = 0;
    virtual DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) = 0;
    virtual SamplerState* createSamplerState(const SamplerDescription& desc) = 0;
    virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) = 0;
    virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format) = 0;
    virtual Texture2D* createTexture2D(PixelFormat format, StreamBuffer& data, const Color& colorKey) = 0;
    virtual Texture2D* createTexture2D(PixelFormat format, StreamBuffer& data) = 0;
    virtual Texture2D* createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) = 0;
    virtual Texture3D* createTexture3D(const std::string& name) = 0;
    virtual TextureCube* createTextureCube(const std::string& name, PixelFormat format) = 0;
    virtual RenderTexture* createRenderTexture(const std::string& name) = 0;

    FillMode getFillMode() const { return mFillMode; }
    void setFillMode(FillMode mode) { mFillMode = mode; }

  protected:
    FillMode mFillMode{FillMode::Fill};
  };
}  // namespace GLaDOS

#endif
