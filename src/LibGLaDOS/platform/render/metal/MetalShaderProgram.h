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
    class MetalShader;
    class MetalShaderProgram : public ShaderProgram {
        friend class MetalRenderer;

      public:
        MetalShaderProgram() = default;
        ~MetalShaderProgram() override;

        MTLRenderPipelineDescriptor* getPipelineDescriptor() const;
        void bindUniforms(MetalRenderable* renderable);
        id<MTLDepthStencilState> metalDepthStencilState();
        MetalRasterizerState* metalRasterizerState();

      private:
        bool createShaderProgram(Shader* vertex, Shader* fragment) override;
        bool makePipelineDescriptor(MetalShader* vertex, MetalShader* fragment);
        MTLVertexDescriptor* makeVertexDescriptor(NSArray<MTLVertexAttribute*>* vertexAttributes);
        bool addShaderArguments(MTLRenderPipelineReflection* pipelineReflection);
        void parseUniform(MTLArgument* argument, ShaderType type);

        MTLRenderPipelineDescriptor* mPipelineDescriptor{nil};
    };
}  // namespace GLaDOS

#endif

#endif