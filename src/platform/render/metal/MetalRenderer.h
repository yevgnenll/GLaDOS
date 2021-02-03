#ifndef GLADOS_METALRENDERER_H
#define GLADOS_METALRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

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
    void render(Renderable* renderable) override;

    Buffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    Buffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    ShaderProgram* createShaderProgram() override;
    Renderable* createRenderable() override;

    id<MTLDevice> getDevice() const;
    id<MTLCommandQueue> getCommandQueue() const;
    id<MTLRenderCommandEncoder> getCommandEncoder() const;
    CAMetalLayer* getMetalLayer() const;

  private:
    id<MTLDevice> mMetalDevice{nullptr};
    id<MTLCommandQueue> mMetalCommandQueue{nullptr};
    id<MTLRenderCommandEncoder> mMetalRenderCommandEncoder{nullptr};
    CAMetalLayer* mMetalLayer{nullptr};
  };
}  // namespace GLaDOS

#endif

#endif
