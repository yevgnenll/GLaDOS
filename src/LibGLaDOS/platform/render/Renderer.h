#ifndef GLADOS_RENDERER_H
#define GLADOS_RENDERER_H

#include <string>
#include "utils/Enumeration.h"
#include "math/Color.h"

namespace GLaDOS {
    class Logger;
    class Blob;
    class Mesh;
    class Material;
    class GPUBuffer;
    class Renderable;
    class ShaderProgram;
    class FrameBuffer;
    class RenderBuffer;
    class VertexBuffer;
    class IndexBuffer;
    class DepthStencilState;
    struct DepthStencilDescription;
    class SamplerState;
    struct SamplerDescription;
    class RasterizerState;
    struct RasterizerDescription;
    class Texture2D;
    class Texture3D;
    class TextureCube;
    class RenderTexture;
    class VertexFormatDescriptor;
    class Shader;
    class Renderer {
      public:
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual bool initialize(int width, int height) = 0;
        virtual void render(Renderable* _renderable) = 0;

        virtual GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) = 0;
        virtual GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) = 0;
        virtual ShaderProgram* createShaderProgram(Shader* vertex, Shader* fragment) = 0;
        virtual ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName) = 0;
        virtual Renderable* createRenderable(Mesh* mesh, Material* material) = 0;
        virtual FrameBuffer* createFrameBuffer() = 0;
        virtual RenderBuffer* createRenderBuffer() = 0;
        virtual DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) = 0;
        virtual SamplerState* createSamplerState(const SamplerDescription& desc) = 0;
        virtual RasterizerState* createRasterizerState(const RasterizerDescription& desc) = 0;
        virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) = 0;
        virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format) = 0;
        virtual Texture2D* createTexture2D(PixelFormat format, Blob& data, const Color& colorKey) = 0;
        virtual Texture2D* createTexture2D(PixelFormat format, Blob& data) = 0;
        virtual Texture2D* createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) = 0;
        virtual Texture3D* createTexture3D(const std::string& name) = 0;
        virtual TextureCube* createTextureCube(const std::string& name, PixelFormat format) = 0;
        virtual RenderTexture* createRenderTexture(const std::string& name) = 0;

        Mesh* createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage);
        Mesh* createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
        Mesh* createMesh(const std::string& meshPath, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage);
        VertexBuffer* createVertexBuffer(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count);

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif
