#ifndef GLADOS_METALSHADERPROGRAM_H
#define GLADOS_METALSHADERPROGRAM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <string>

#include "MetalRenderer.h"
#include "memory/Blob.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/VertexFormat.h"

namespace GLaDOS {
    class Logger;
    class MetalRenderable;
    class MetalDepthStencilState;
    class MetalRasterizerState;
    class VertexFormatHolder;
    class MetalShaderProgram : public ShaderProgram {
        friend class MetalRenderer;

      public:
        MetalShaderProgram() = default;
        ~MetalShaderProgram() override;

        MTLRenderPipelineDescriptor* getPipelineDescriptor() const;
        void bindUniforms(MetalRenderable* _renderable);
        MTLVertexDescriptor* makeVertexDescriptor(VertexFormatHolder* vertexFormatHolder);
        id<MTLDepthStencilState> metalDepthStencilState();
        MetalRasterizerState* metalRasterizerState();

      private:
        bool createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexData* vertexData) override;

        MTLVertexAttribute* findVertexAttribute(VertexSemantic semantic);
        bool makePipelineDescriptor(const VertexData* vertexData);
        bool addShaderArguments(MTLRenderPipelineReflection* pipelineReflection);
        void addUniform(MTLArgument* argument, ShaderType type);
        void reserveUniformMemory();

        static constexpr MTLVertexFormat mapVertexFormatFrom(VertexAttributeType type);
        static constexpr UniformType mapUniformTypeFrom(MTLDataType dataType);
        static constexpr std::size_t mapUniformTypeSizeForm(UniformType uniformType);
        static constexpr const char* mapAttributeNameFrom(VertexSemantic semantic);
        static constexpr MTLTriangleFillMode mapFillModeFrom(FillMode mode);
        static constexpr MTLWinding mapWindingModeFrom(WindingMode mode);
        static constexpr MTLCullMode mapCullModeFrom(CullMode mode);
        static bool createShader(const std::string& source, id<MTLFunction>& function);
        static Logger* logger;

        id<MTLFunction> mVertexFunction{nil};
        id<MTLFunction> mFragmentFunction{nil};
        MTLRenderPipelineDescriptor* mPipelineDescriptor{nil};
        Blob mVertexUniformBuffer;
        Blob mFragmentUniformBuffer;
    };
}  // namespace GLaDOS

#endif

#endif