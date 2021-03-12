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
    ShaderProgram* createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) override;
    Renderable* createRenderable(Mesh* mesh, Material* material) override;
    Mesh* createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) override;
    Mesh* createMesh(const std::string& meshPath, PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex) override;
    FrameBuffer* createFrameBuffer() override;
    RenderBuffer* createRenderBuffer() override;
    DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
    SamplerState* createSamplerState(const SamplerDescription& desc) override;

    id<MTLDevice> getDevice() const;
    id<MTLRenderCommandEncoder> getCommandEncoder() const;
    void setCommandEncoder(id<MTLRenderCommandEncoder> commandEncoder);
    CAMetalLayer* getMetalLayer() const;

  private:
    static MTLPrimitiveType mapPrimitiveType(PrimitiveType type);
    static MTLIndexType mapIndexType(int size);

    id<MTLDevice> mMetalDevice{nil};
    id<MTLRenderCommandEncoder> mCommandEncoder{nil};
    CAMetalLayer* mMetalLayer{nullptr};
  };
}  // namespace GLaDOS

#endif

#endif
