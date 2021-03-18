#ifndef GLADOS_METALRENDERER_H
#define GLADOS_METALRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

#include "memory/StreamBuffer.h"
#include "platform/render/Renderer.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
  class Buffer;
  class MetalRenderer : public Renderer, public Singleton<MetalRenderer> {
  public:
    MetalRenderer() = default;
    ~MetalRenderer() override;

    bool initialize() override;
    void render(Renderable* _renderable) override;

    Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    ShaderProgram* createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const VertexData* vertexData) override;
    Renderable* createRenderable(Mesh* mesh, Material* material) override;
    Mesh* createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) override;
    Mesh* createMesh(const std::string& meshPath, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) override;
    FrameBuffer* createFrameBuffer() override;
    RenderBuffer* createRenderBuffer() override;
    DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
    SamplerState* createSamplerState(const SamplerDescription& desc) override;
    Texture2D* createTexture2D(const std::string& name, TextureFormat format, const Color& colorKey) override;
    Texture2D* createTexture2D(const std::string& name, TextureFormat format) override;
    Texture2D* createTexture2D(TextureFormat format, StreamBuffer& data, const Color& colorKey) override;
    Texture2D* createTexture2D(TextureFormat format, StreamBuffer& data) override;
    Texture2D* createTexture2D(uint32_t width, uint32_t height, TextureFormat format, unsigned char* data) override;
    Texture3D* createTexture3D(const std::string& name) override;
    TextureCube* createTextureCube(const std::string& name) override;
    RenderTexture* createRenderTexture(const std::string& name) override;

    id<MTLDevice> getDevice() const;
    id<MTLRenderCommandEncoder> getCommandEncoder() const;
    void setCommandEncoder(id<MTLRenderCommandEncoder> commandEncoder);
    CAMetalLayer* getMetalLayer() const;

  private:
    static MTLPrimitiveType mapPrimitiveType(PrimitiveType type);
    static MTLIndexType mapIndexType(std::size_t size);

    id<MTLDevice> mMetalDevice{nil};
    id<MTLRenderCommandEncoder> mCommandEncoder{nil};
    CAMetalLayer* mMetalLayer{nil};
  };
}  // namespace GLaDOS

#endif

#endif
