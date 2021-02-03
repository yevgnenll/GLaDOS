#include "MetalRenderer.h"

#ifdef PLATFORM_MACOS

#include "MetalBuffer.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"

namespace GLaDOS {
  MetalRenderer::~MetalRenderer() {
    [mMetalCommandQueue release];
    [mMetalDevice release];
  }

  bool MetalRenderer::initialize() {
    mMetalDevice = MTLCreateSystemDefaultDevice();
    if (mMetalDevice == nullptr) {
      LOG_ERROR("System does not support metal.");
      return false;
    }

    mMetalLayer = [CAMetalLayer layer];
    if (mMetalLayer == nullptr) {
      LOG_ERROR("System does not support metal layer.");
      return false;
    }
    mMetalLayer.device = mMetalDevice;
    mMetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;

    mMetalCommandQueue = [mMetalDevice newCommandQueue];
    if (mMetalCommandQueue == nullptr) {
      LOG_ERROR("System does not support metal command queue.");
      return false;
    }

    return true;
  }

  void MetalRenderer::render(Renderable* renderable) {
    id<CAMetalDrawable> drawable = [mMetalLayer nextDrawable];
    id<MTLTexture> texture = drawable.texture;

    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.f, 0.0f, 0.0f, 1.0f);

    id<MTLCommandBuffer> commandBuffer = [mMetalCommandQueue commandBuffer];

    mMetalRenderCommandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    [mMetalRenderCommandEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
  }

  Buffer* MetalRenderer::createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) {
    Buffer* vertexBuffer = NEW_T(MetalBuffer(BufferType::VertexBuffer, usage));
    vertexBuffer->uploadData(buffer);
    return vertexBuffer;
  }

  Buffer* MetalRenderer::createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) {
    Buffer* indexBuffer = NEW_T(MetalBuffer(BufferType::IndexBuffer, usage));
    indexBuffer->uploadData(buffer);
    return indexBuffer;
  }

  ShaderProgram* MetalRenderer::createShaderProgram() {
    return NEW_T(MetalShaderProgram);
  }

  Renderable* MetalRenderer::createRenderable() {
    static std::atomic<RenderableId> id = 0;
    Renderable* renderable = NEW_T(MetalRenderable(id++));
    mRenderable.try_emplace(id.load(), renderable);
    return renderable;
  }

  id<MTLDevice> MetalRenderer::getDevice() const {
    return mMetalDevice;
  }

  id<MTLCommandQueue> MetalRenderer::getCommandQueue() const {
    return mMetalCommandQueue;
  }

  id<MTLRenderCommandEncoder> MetalRenderer::getCommandEncoder() const {
    return mMetalRenderCommandEncoder;
  }

  CAMetalLayer* MetalRenderer::getMetalLayer() const {
    return mMetalLayer;
  }
}  // namespace GLaDOS

#endif