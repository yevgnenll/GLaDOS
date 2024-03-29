#ifndef GLADOS_RENDERER_H
#define GLADOS_RENDERER_H

#include <string>
#include "utils/Enumeration.h"
#include "math/Color.h"
#include "utils/Stl.h"

namespace GLaDOS {
    class Logger;
    class Blob;
    class Mesh;
    class Material;
    class GPUBuffer;
    class Renderable;
    class ShaderProgram;
    class FrameBuffer;
    class VertexBuffer;
    class IndexBuffer;
    class DepthStencilState;
    struct DepthStencilDescription;
    class SamplerState;
    struct SamplerDescription;
    class RasterizerState;
    struct RasterizerDescription;
    class RenderPipelineState;
    struct RenderPipelineDescription;
    class Texture2D;
    class Texture3D;
    class TextureCube;
    class RenderTexture;
    class VertexFormatDescriptor;
    class Shader;
    template <typename T>
    class Rect;
    class GameObject;
    class Renderer {
      public:
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual bool initialize(int width, int height) = 0;
        virtual void render(Renderable* _renderable, const Rect<real>& normalizedViewportRect) = 0;

        virtual GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) = 0;
        virtual GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) = 0;
        virtual ShaderProgram* createShaderProgram(Shader* vertex, Shader* fragment, RenderPipelineState* renderPipelineState = nullptr) = 0;
        virtual ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, RenderPipelineState* renderPipelineState = nullptr) = 0;
        virtual ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, RenderPipelineState* renderPipelineState = nullptr) = 0;
        virtual Renderable* createRenderable(Mesh* mesh, Material* material) = 0;
        virtual FrameBuffer* createFrameBuffer() = 0;
        virtual DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) = 0;
        virtual SamplerState* createSamplerState(const SamplerDescription& desc) = 0;
        virtual RasterizerState* createRasterizerState(const RasterizerDescription& desc) = 0;
        virtual RenderPipelineState* createRenderPipelineState(const RenderPipelineDescription& desc) = 0;
        virtual RenderTexture* createRenderTexture(uint32_t width, uint32_t height, PixelFormat format) = 0;
        virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format) = 0;
        virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format, Blob& data) = 0;
        virtual Texture2D* createTexture2D(const std::string& name, PixelFormat format, unsigned char* data) = 0;
        virtual Texture3D* createTexture3D(const std::string& name) = 0;
        virtual TextureCube* createTextureCube(const std::string& name, const Array<std::string, 6>& cubeNames, PixelFormat format) = 0;

        Mesh* createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology);
        Mesh* createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage);
        Mesh* createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
        bool createPrefabFromFile(const std::string& meshFilePath, GameObject* parent);
        VertexBuffer* createVertexBuffer(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count);
        IndexBuffer* createIndexBuffer(std::size_t stride, std::size_t count);

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif
