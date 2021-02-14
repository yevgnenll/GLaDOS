#include "MetalRenderer.h"

#ifdef PLATFORM_MACOS

#include "MetalBuffer.h"
#include "MetalFrameBuffer.h"
#include "MetalRenderBuffer.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"
#include "platform/render/Mesh.h"
#include "utils/FileSystem.h"

namespace GLaDOS {
  MetalRenderer::~MetalRenderer() {
    [mMetalDevice release];
    [mMetalLayer release];
  }

  bool MetalRenderer::initialize() {
    mMetalDevice = MTLCreateSystemDefaultDevice();
    if (nil == mMetalDevice) {
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
    mMetalLayer.allowsNextDrawableTimeout = NO;
    // these properties are crucial to resizing working
    mMetalLayer.autoresizingMask = kCALayerHeightSizable | kCALayerWidthSizable;
    mMetalLayer.needsDisplayOnBoundsChange = YES;
    mMetalLayer.presentsWithTransaction = YES;

    return true;
  }

  void MetalRenderer::render(Renderable* _renderable) {
    if (_renderable == nullptr) {
      return;
    }
    MetalRenderable* renderable = static_cast<MetalRenderable*>(_renderable);  // INTEND: do not use dynamic_cast here
    renderable->bindParams();

    Mesh* mesh = renderable->getMesh();
    Buffer* indexBuffer = mesh->getIndexBuffer();
    MTLPrimitiveType primitiveType = MetalRenderer::mapPrimitiveType(mesh->getPrimitiveType());

    [mCommandEncoder setRenderPipelineState:renderable->getPipelineState()];
    if (indexBuffer != nullptr) {
      // index primitive draw
      MTLIndexType indexType = MetalRenderer::mapIndexType(sizeof(mesh->getIndexStride()));
      uint32_t indexCount = mesh->getIndexCount();
      NSUInteger indexOffset = mesh->getIndexStart() * mesh->getIndexStride();

      [mCommandEncoder setVertexBuffer:renderable->getVertexBuffer() offset:0 atIndex:1];
      [mCommandEncoder drawIndexedPrimitives:primitiveType indexCount:indexCount indexType:indexType indexBuffer:renderable->getIndexBuffer() indexBufferOffset:indexOffset];
      return;
    }

    // vertex primitive draw
    uint32_t start = mesh->getVertexStart();
    uint32_t count = mesh->getVertexCount();

    [mCommandEncoder setVertexBuffer:renderable->getVertexBuffer() offset:0 atIndex:1];
    [mCommandEncoder drawPrimitives:primitiveType vertexStart:start vertexCount:count];
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

  ShaderProgram* MetalRenderer::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    MetalShaderProgram* shaderProgram = NEW_T(MetalShaderProgram);
    std::string shaderDirectory = shaderProgram->directory();

    FileSystem vertexFile{shaderDirectory + vertexPath, OpenMode::ReadBinary};
    std::string vertexSource;
    if (!vertexFile.readAll(vertexSource)) {
      LOG_ERROR("Vertex shader {0} is not found.", vertexPath);
      return nullptr;
    }

    FileSystem fragmentFile{shaderDirectory + fragmentPath, OpenMode::ReadBinary};
    std::string fragmentSource;
    if (!fragmentFile.readAll(fragmentSource)) {
      LOG_ERROR("Fragment shader {0} is not found.", fragmentPath);
      return nullptr;
    }

    if (!shaderProgram->createShaderProgram(vertexSource, fragmentSource)) {
      return nullptr;
    }

    return shaderProgram;
  }

  Renderable* MetalRenderer::createRenderable(Mesh* mesh, Material* material) {
    Renderable* renderable = NEW_T(MetalRenderable);
    renderable->mMesh = mesh;
    renderable->mMaterial = material;
    renderable->build();

    return renderable;
  }

  FrameBuffer* MetalRenderer::createFrameBuffer() {
    return NEW_T(MetalFrameBuffer);
  }

  RenderBuffer* MetalRenderer::createRenderBuffer() {
    return NEW_T(MetalRenderBuffer);
  }

  id<MTLDevice> MetalRenderer::getDevice() const {
    return mMetalDevice;
  }

  id<MTLRenderCommandEncoder> MetalRenderer::getCommandEncoder() const {
    return mCommandEncoder;
  }

  void MetalRenderer::setCommandEncoder(id<MTLRenderCommandEncoder> commandEncoder) {
    mCommandEncoder = commandEncoder;
  }

  CAMetalLayer* MetalRenderer::getMetalLayer() const {
    return mMetalLayer;
  }

  MTLPrimitiveType MetalRenderer::mapPrimitiveType(PrimitiveType type) {
    switch (type) {
      case PrimitiveType::Point:
        return MTLPrimitiveTypePoint;
      case PrimitiveType::Line:
        return MTLPrimitiveTypeLine;
      case PrimitiveType::LineStrip:
        return MTLPrimitiveTypeLineStrip;
      case PrimitiveType::Triangle:
        return MTLPrimitiveTypeTriangle;
      case PrimitiveType::TriangleStrip:
        return MTLPrimitiveTypeTriangleStrip;
    }
  }

  MTLIndexType MetalRenderer::mapIndexType(int size) {
    switch (size) {
      case sizeof(uint32_t):
        return MTLIndexTypeUInt32;
      case sizeof(uint16_t):
        return MTLIndexTypeUInt16;
      default:
        break;
    }

    return MTLIndexTypeUInt16;
  }
}  // namespace GLaDOS

#endif