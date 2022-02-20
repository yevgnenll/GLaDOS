#ifndef GLADOS_METALRENDERER_H
#define GLADOS_METALRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

#include "memory/Blob.h"
#include "platform/render/Renderer.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
    class Logger;
    class GPUBuffer;
    class MetalRenderer : public Renderer, public Singleton<MetalRenderer> {
      public:
        MetalRenderer();
        ~MetalRenderer() override;

        bool initialize(int width, int height) override;
        void render(Renderable* _renderable, const Rect<real>& normalizedViewportRect) override;

        GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        ShaderProgram* createShaderProgram(Shader* vertex, Shader* fragment, RenderPipelineState* renderPipelineState) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, RenderPipelineState* renderPipelineState) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, RenderPipelineState* renderPipelineState) override;
        Renderable* createRenderable(Mesh* mesh, Material* material) override;
        FrameBuffer* createFrameBuffer() override;
        RenderBuffer* createRenderBuffer() override;
        DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
        SamplerState* createSamplerState(const SamplerDescription& desc) override;
        RasterizerState* createRasterizerState(const RasterizerDescription& desc) override;
        RenderPipelineState* createRenderPipelineState(const RenderPipelineDescription& desc) override;
        Texture2D* createRenderTexture2D(const std::string& name, uint32_t width, uint32_t height, PixelFormat format) override;
        TextureCube* createRenderTextureCube(const std::string& name, uint32_t width, uint32_t height, PixelFormat format) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, Blob& data) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, unsigned char* data) override;
        Texture3D* createTexture3D(const std::string& name) override;
        TextureCube* createTextureCube(const std::string& name, const Array<std::string, 6>& cubeNames, PixelFormat format) override;

        id<MTLDevice> getDevice() const;
        id<MTLCommandQueue> getCommandQueue() const;
        id<MTLRenderCommandEncoder> getCommandEncoder() const;
        void setCommandEncoder(id<MTLRenderCommandEncoder> commandEncoder);
        CAMetalLayer* getMetalLayer() const;

      private:
        static Logger* logger;

        id<MTLDevice> mMetalDevice{nil};
        id<MTLCommandQueue> mCommandQueue{nil};
        id<MTLRenderCommandEncoder> mCommandEncoder{nil};
        CAMetalLayer* mMetalLayer{nil};
    };
}  // namespace GLaDOS

#endif

#endif
