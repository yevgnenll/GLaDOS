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
        void render(Renderable* _renderable) override;

        GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        ShaderProgram* createShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName) override;
        Renderable* createRenderable(Mesh* mesh, Material* material) override;
        FrameBuffer* createFrameBuffer() override;
        RenderBuffer* createRenderBuffer() override;
        DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
        SamplerState* createSamplerState(const SamplerDescription& desc) override;
        RasterizerState* createRasterizerState(const RasterizerDescription& desc) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format) override;
        Texture2D* createTexture2D(PixelFormat format, Blob& data, const Color& colorKey) override;
        Texture2D* createTexture2D(PixelFormat format, Blob& data) override;
        Texture2D* createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) override;
        Texture3D* createTexture3D(const std::string& name) override;
        TextureCube* createTextureCube(const std::string& name, PixelFormat format) override;
        RenderTexture* createRenderTexture(const std::string& name) override;

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
