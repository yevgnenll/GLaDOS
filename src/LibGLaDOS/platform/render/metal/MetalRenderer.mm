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
#include "MetalTypes.h"
#include "platform/render/Mesh.h"
#include "platform/render/VertexBuffer.h"
#include "resource/ResourceManager.h"
#include "utils/FileSystem.h"
#include "MetalShader.h"
#include "RootDir.h"
#include "math/Rect.hpp"

namespace GLaDOS {
    Logger* MetalRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalRenderer");

    MetalRenderer::MetalRenderer() {
        setDestructionPhase(2);
    }

    MetalRenderer::~MetalRenderer() {
        [mCommandQueue release];
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
        // The default is NO, which indicates the use of the texture is not restricted.
        mMetalLayer.framebufferOnly = NO;
        // If YES, the nextDrawable method returns nil if it can’t provide a drawable object within one second.
        // If NO, the nextDrawable method waits indefinitely for a drawable to become available.
        mMetalLayer.allowsNextDrawableTimeout = NO;

        mCommandQueue = [mMetalDevice newCommandQueue];

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

    void MetalRenderer::render(Renderable* _renderable, const Rect<real>& normalizedViewportRect) {
        if (_renderable == nullptr) {
            return;
        }
        MetalRenderable* renderable = static_cast<MetalRenderable*>(_renderable);  // INTEND: do not use dynamic_cast here
        renderable->bindParams();

        Mesh* mesh = renderable->getMesh();
        GPUBuffer* indexBuffer = mesh->getGPUIndexBuffer();
        MTLPrimitiveType primitiveType = MetalTypes::primitiveTopologyToMetal(mesh->getPrimitiveType());

        [mCommandEncoder setRenderPipelineState:renderable->getPipelineState()];
        [mCommandEncoder setVertexBuffer:renderable->getVertexBuffer() offset:0 atIndex:1];

        // change viewport of camera
        CGSize renderTargetSize = [mMetalLayer drawableSize];
        CGFloat renderTargetWidth = renderTargetSize.width; // scaleFactor 가 적용된 넓이
        CGFloat renderTargetHeight = renderTargetSize.height; // scaleFactor 가 적용된 높이
        Rect<double> viewportRect;
        viewportRect.x = normalizedViewportRect.x * renderTargetWidth;
        viewportRect.y = normalizedViewportRect.y * renderTargetHeight;
        viewportRect.w = normalizedViewportRect.w * renderTargetWidth;
        viewportRect.h = normalizedViewportRect.h * renderTargetHeight;

        /*
            Metal viewport coordinate (framebuffer coordinate)
            The origin (0, 0) is located at the top-left corner (Y down)
            (0, 0)                 (1024, 0)
              +--------------------+ +X
              |                    |
              |                    |
              |                    |
              |                    |
              |                    |
              |                    |
              +--------------------+
              +Y (0, 800)
        */
        MTLViewport viewport;
        viewport.originX = viewportRect.x;
        viewport.originY = renderTargetHeight - viewportRect.y - viewportRect.h; // convert to Y up
        viewport.width = viewportRect.w;
        viewport.height = viewportRect.h;
        viewport.znear = 0;
        viewport.zfar = 1;
        [mCommandEncoder setViewport:viewport];

        if (indexBuffer != nullptr) {
            // index primitive draw
            MTLIndexType indexType = MetalTypes::sizeToMetalIndexType(mesh->getIndexStride());
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

    GPUBuffer* MetalRenderer::createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* vertexBuffer = NEW_T(MetalGPUBuffer(GPUBufferType::VertexBuffer, usage));
        if (!vertexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create vertex buffer");
            return nullptr;
        }

        return vertexBuffer;
    }

    GPUBuffer* MetalRenderer::createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* indexBuffer = NEW_T(MetalGPUBuffer(GPUBufferType::IndexBuffer, usage));
        if (!indexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create index buffer");
            return nullptr;
        }

        return indexBuffer;
    }

    ShaderProgram* MetalRenderer::createShaderProgram(Shader* vertex, Shader* fragment) {
        MetalShaderProgram* shaderProgram = NEW_T(MetalShaderProgram);

        if (!shaderProgram->createShaderProgram(vertex, fragment)) {
            DELETE_T(shaderProgram, MetalShaderProgram);
            LOG_ERROR(logger, "Shader compilation error");
            return nullptr;
        }

        return shaderProgram;
    }

    ShaderProgram* MetalRenderer::createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName) {
        // Load Vertex Resource
        Resource* vertexResource = ResourceManager::getInstance().getResource(vertexName, ResourceType::Shader);
        if (vertexResource == nullptr) {
            FileSystem vertexFile{SHADER_DIR + vertexName + SHADER_SUFFIX, OpenMode::ReadBinary};
            std::string vertexSource;
            if (!vertexFile.readAll(vertexSource)) {
                LOG_ERROR(logger, "Vertex shader {0} is not found.", vertexFile.fullName());
                return nullptr;
            }

            MetalShader* vertexShader = NEW_T(MetalShader(vertexSource));
            vertexShader->setName(vertexName);
            if (!vertexShader->createShader()) {
                DELETE_T(vertexShader, MetalShader);
                return nullptr;
            }

            if (!ResourceManager::getInstance().store(vertexShader)) {
                DELETE_T(vertexShader, MetalShader);
                return nullptr;
            }

            vertexResource = vertexShader;
        }

        // Load Fragment Resource
        Resource* fragmentResource = ResourceManager::getInstance().getResource(fragmentName, ResourceType::Shader);
        if (fragmentResource == nullptr) {
            FileSystem fragmentFile{SHADER_DIR + fragmentName + SHADER_SUFFIX, OpenMode::ReadBinary};
            std::string fragmentSource;
            if (!fragmentFile.readAll(fragmentSource)) {
                LOG_ERROR(logger, "Fragment shader {0} is not found.", fragmentFile.fullName());
                return nullptr;
            }

            MetalShader* fragmentShader = NEW_T(MetalShader(fragmentSource));
            fragmentShader->setName(fragmentName);
            if (!fragmentShader->createShader()) {
                DELETE_T(fragmentShader, MetalShader);
                return nullptr;
            }

            if (!ResourceManager::getInstance().store(fragmentShader)) {
                DELETE_T(fragmentShader, MetalShader);
                return nullptr;
            }

            fragmentResource = fragmentShader;
        }

        MetalShaderProgram* shaderProgram = NEW_T(MetalShaderProgram);
        if (!shaderProgram->createShaderProgram(static_cast<Shader*>(vertexResource), static_cast<Shader*>(fragmentResource))) {
            DELETE_T(shaderProgram, MetalShaderProgram);
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
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Texture);
        if (resource != nullptr) {
            return static_cast<Texture2D*>(resource);
        }

        MetalTexture2D* texture = NEW_T(MetalTexture2D(name, format));
        texture->setColorKey(colorKey);
        if (!texture->loadTextureFromFile()) {
            DELETE_T(texture, MetalTexture2D);
            return nullptr;
        }

        ResourceManager::getInstance().store(texture);

        return texture;
    }

    Texture2D* MetalRenderer::createTexture2D(const std::string& name, PixelFormat format) {
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Texture);
        if (resource != nullptr) {
            return static_cast<Texture2D*>(resource);
        }

        MetalTexture2D* texture = NEW_T(MetalTexture2D(name, format));
        if (!texture->loadTextureFromFile()) {
            DELETE_T(texture, MetalTexture2D);
            return nullptr;
        }

        ResourceManager::getInstance().store(texture);

        return texture;
    }

    Texture2D* MetalRenderer::createTexture2D(PixelFormat format, Blob& data, const Color& colorKey) {
        // TODO
        return nullptr;
    }

    Texture2D* MetalRenderer::createTexture2D(PixelFormat format, Blob& data) {
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

    TextureCube* MetalRenderer::createTextureCube(const std::string& name, const Array<std::string, 6>& cubeNames, PixelFormat format) {
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Texture);
        if (resource != nullptr) {
            return static_cast<TextureCube*>(resource);
        }

        TextureCube* textureCube = NEW_T(MetalTextureCube(name, format));
        if (!textureCube->loadTextureFromFile(cubeNames)) {
            DELETE_T(textureCube, TextureCube);
            return nullptr;
        }

        ResourceManager::getInstance().store(textureCube);

        return textureCube;
    }

    RenderTexture* MetalRenderer::createRenderTexture(const std::string& name) {
        // TODO
        return nullptr;
    }

    id<MTLDevice> MetalRenderer::getDevice() const {
        return mMetalDevice;
    }

    id<MTLCommandQueue> MetalRenderer::getCommandQueue() const {
        return mCommandQueue;
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
}  // namespace GLaDOS

#endif