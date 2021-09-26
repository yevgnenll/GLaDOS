#include "MetalRenderer.h"

#ifdef PLATFORM_MACOS

#include "MetalFrameBuffer.h"
#include "MetalGPUBuffer.h"
#include "MetalRenderBuffer.h"
#include "MetalRenderState.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"
#include "MetalTexture2D.h"
#include "MetalTextureCube.h"
#include "platform/render/Mesh.h"
#include "platform/render/VertexData.h"
#include "utils/FileSystem.h"
#include "resource/ResourceManager.h"

namespace GLaDOS {
    Logger* MetalRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalRenderer");

    MetalRenderer::MetalRenderer() {
        setDestructionPhase(2);
    }

    MetalRenderer::~MetalRenderer() {
        [mMetalDevice release];
        [mMetalLayer release];
    }

    bool MetalRenderer::initialize(int width, int height) {
        mMetalDevice = MTLCreateSystemDefaultDevice();
        if (mMetalDevice == nil) {
            LOG_ERROR(logger, "System does not support metal.");
            return false;
        }

        mMetalLayer = [CAMetalLayer layer];
        if (mMetalLayer == nil) {
            LOG_ERROR(logger, "System does not support metal layer.");
            return false;
        }

        mMetalLayer.device = mMetalDevice;
        mMetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        // If YES, the nextDrawable method returns nil if it can’t provide a drawable object within one second.
        // If NO, the nextDrawable method waits indefinitely for a drawable to become available.
        mMetalLayer.allowsNextDrawableTimeout = NO;

        LOG_TRACE(logger, "MetalRenderer supports MSAA sample count 1: {0}", static_cast<bool>([mMetalDevice supportsTextureSampleCount:1]));
        LOG_TRACE(logger, "MetalRenderer supports MSAA sample count 2: {0}", static_cast<bool>([mMetalDevice supportsTextureSampleCount:2]));
        LOG_TRACE(logger, "MetalRenderer supports MSAA sample count 4: {0}", static_cast<bool>([mMetalDevice supportsTextureSampleCount:4]));
        LOG_TRACE(logger, "MetalRenderer supports MSAA sample count 8: {0}", static_cast<bool>([mMetalDevice supportsTextureSampleCount:8]));
        LOG_TRACE(logger, "MetalRenderer supports argument buffers: {0}", static_cast<bool>([mMetalDevice argumentBuffersSupport]));
        if (static_cast<int>([mMetalDevice respondsToSelector:@selector(supports32BitMSAA)]) != 0) {
            LOG_TRACE(logger, "MetalRenderer supports 32 bits floating point MSAA: {0}", static_cast<bool>([mMetalDevice supports32BitMSAA]));
        }
        if (static_cast<int>([mMetalDevice respondsToSelector:@selector(supportsBCTextureCompression)]) != 0) {
            LOG_TRACE(logger, "MetalRenderer supports BC Texture compression format: {0}", static_cast<bool>([mMetalDevice supportsBCTextureCompression]));
        }
#if defined(ARCH_ARM_CPU)
        LOG_TRACE(logger, "Target Apple Silicon");
#elif defined(ARCH_INTEL_CPU)
        LOG_TRACE(logger, "Target Intel CPU");
#endif

        LOG_TRACE(logger, "MetalRenderer init success with Graphics Card: {0}", [[mMetalDevice name] UTF8String]);

        return true;
    }

    void MetalRenderer::render(Renderable* _renderable) {
        if (_renderable == nullptr) {
            return;
        }
        MetalRenderable* renderable = static_cast<MetalRenderable*>(_renderable);  // INTEND: do not use dynamic_cast here
        renderable->bindParams();

        Mesh* mesh = renderable->getMesh();
        GPUBuffer* indexBuffer = mesh->getIndexBuffer();
        MTLPrimitiveType primitiveType = MetalRenderer::mapPrimitiveType(mesh->getPrimitiveType());

        [mCommandEncoder setRenderPipelineState:renderable->getPipelineState()];
        [mCommandEncoder setVertexBuffer:renderable->getVertexBuffer() offset:0 atIndex:1];
        if (indexBuffer != nullptr) {
            // index primitive draw
            MTLIndexType indexType = MetalRenderer::mapIndexType(mesh->getIndexStride());
            std::size_t indexCount = mesh->getIndexCount();
            NSUInteger indexOffset = mesh->getIndexStart() * mesh->getIndexStride();
            [mCommandEncoder drawIndexedPrimitives:primitiveType indexCount:indexCount indexType:indexType indexBuffer:renderable->getIndexBuffer() indexBufferOffset:indexOffset];
            return;
        }

        // vertex primitive draw
        std::size_t start = mesh->getVertexStart();
        std::size_t count = mesh->getVertexCount();
        [mCommandEncoder drawPrimitives:primitiveType vertexStart:start vertexCount:count];
    }

    GPUBuffer* MetalRenderer::createVertexBuffer(BufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* vertexBuffer = NEW_T(MetalGPUBuffer(BufferType::VertexBuffer, usage));
        if (!vertexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create vertex buffer");
            return nullptr;
        }

        return vertexBuffer;
    }

    GPUBuffer* MetalRenderer::createIndexBuffer(BufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* indexBuffer = NEW_T(MetalGPUBuffer(BufferType::IndexBuffer, usage));
        if (!indexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create index buffer");
            return nullptr;
        }

        return indexBuffer;
    }

    ShaderProgram* MetalRenderer::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const VertexData* vertexData) {
        MetalShaderProgram* shaderProgram = NEW_T(MetalShaderProgram);
        std::string shaderDirectory = shaderProgram->directory();

        FileSystem vertexFile{shaderDirectory + vertexPath, OpenMode::ReadBinary};
        std::string vertexSource;
        if (!vertexFile.readAll(vertexSource)) {
            LOG_ERROR(logger, "Vertex shader {0} is not found.", vertexPath);
            return nullptr;
        }

        FileSystem fragmentFile{shaderDirectory + fragmentPath, OpenMode::ReadBinary};
        std::string fragmentSource;
        if (!fragmentFile.readAll(fragmentSource)) {
            LOG_ERROR(logger, "Fragment shader {0} is not found.", fragmentPath);
            return nullptr;
        }

        if (!shaderProgram->createShaderProgram(vertexSource, fragmentSource, vertexData)) {
            LOG_ERROR(logger, "Shader compilation error");
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

    Mesh* MetalRenderer::createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, BufferUsage vertexUsage, BufferUsage indexUsage) {
        Mesh* mesh = NEW_T(Mesh(primitiveType, vertexUsage, indexUsage));
        if (!mesh->build(vertexData, indexData)) {
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        return mesh;
    }

    Mesh* MetalRenderer::createMesh(VertexData* vertexData, IndexData* indexData) {
        Mesh* mesh = NEW_T(Mesh);
        if (!mesh->build(vertexData, indexData)) {
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        return mesh;
    }

    Mesh* MetalRenderer::createMesh(const std::string& meshPath, PrimitiveType primitiveType, BufferUsage vertexUsage, BufferUsage indexUsage) {
        // TODO
        // const auto& [vertexData, indexData] = MeshLoader::loadFromFile(meshPath);
        // return createMesh(vertexData, indexData, primitiveType, dynamicVertex, dynamicIndex);
        return nullptr;
    }

    FrameBuffer* MetalRenderer::createFrameBuffer() {
        return NEW_T(MetalFrameBuffer);
    }

    RenderBuffer* MetalRenderer::createRenderBuffer() {
        return NEW_T(MetalRenderBuffer);
    }

    DepthStencilState* MetalRenderer::createDepthStencilState(const DepthStencilDescription& desc) {
        return NEW_T(MetalDepthStencilState(desc));
    }

    SamplerState* MetalRenderer::createSamplerState(const SamplerDescription& desc) {
        return NEW_T(MetalSamplerState(desc));
    }

    RasterizerState* MetalRenderer::createRasterizerState(const RasterizerDescription& desc) {
        return NEW_T(MetalRasterizerState(desc));
    }

    Texture2D* MetalRenderer::createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) {
        MetalTexture2D* texture = NEW_T(MetalTexture2D(name, format));
        texture->setColorKey(colorKey);
        ResourceManager::getInstance().store(texture);
        return texture;
    }

    Texture2D* MetalRenderer::createTexture2D(const std::string& name, PixelFormat format) {
        MetalTexture2D* texture = NEW_T(MetalTexture2D(name, format));
        ResourceManager::getInstance().store(texture);
        return texture;
    }

    Texture2D* MetalRenderer::createTexture2D(PixelFormat format, StreamBuffer& data, const Color& colorKey) {
        // TODO
        return nullptr;
    }

    Texture2D* MetalRenderer::createTexture2D(PixelFormat format, StreamBuffer& data) {
        // TODO
        return nullptr;
    }

    Texture2D* MetalRenderer::createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) {
        // TODO
        return nullptr;
    }

    Texture3D* MetalRenderer::createTexture3D(const std::string& name) {
        // TODO
        return nullptr;
    }

    TextureCube* MetalRenderer::createTextureCube(const std::string& name, PixelFormat format) {
        TextureCube* textureCube = NEW_T(MetalTextureCube(name, format));
        ResourceManager::getInstance().store(textureCube);
        return textureCube;
    }

    RenderTexture* MetalRenderer::createRenderTexture(const std::string& name) {
        // TODO
        return nullptr;
    }

    VertexData* MetalRenderer::createVertexData(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count) {
        VertexData* vertexData = NEW_T(VertexData(vertexFormatDescriptor, count));
        return vertexData;
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

    MTLIndexType MetalRenderer::mapIndexType(std::size_t size) {
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