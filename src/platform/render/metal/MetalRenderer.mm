#include "MetalRenderer.h"

#ifdef PLATFORM_MACOS

#include "MetalBuffer.h"
#include "MetalShaderProgram.h"
#include "utils/Utility.h"

namespace GameEngine {
  MetalRenderer* MetalRenderer::instance = nullptr;

  MetalRenderer::MetalRenderer() {
    MetalRenderer::instance = this;
  }

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

  void MetalRenderer::render() const {
  }

  Buffer* MetalRenderer::createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) {
    return NEW_T(MetalBuffer(BufferType::VertexBuffer, usage, buffer));
  }

  Buffer* MetalRenderer::createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) {
    return NEW_T(MetalBuffer(BufferType::IndexBuffer, usage, buffer));
  }

  ShaderProgram* MetalRenderer::createShaderProgram() {
    return NEW_T(MetalShaderProgram);
  }

  id<MTLDevice> MetalRenderer::getMetalDevice() const {
    return mMetalDevice;
  }

  id<MTLCommandQueue> MetalRenderer::getMetalCommandQueue() const {
    return mMetalCommandQueue;
  }

  CAMetalLayer* MetalRenderer::getMetalLayer() const {
    return mMetalLayer;
  }

  MetalRenderer* MetalRenderer::getInstance() {
    return instance;
  }
}  // namespace GameEngine

#endif