#ifndef GLADOS_METALSHADERPROGRAM_H
#define GLADOS_METALSHADERPROGRAM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/Metal.h>

#include <string>

#include "memory/StreamBuffer.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/VertexFormat.h"

namespace GLaDOS {
  class MetalRenderable;
  class MetalShaderProgram : public ShaderProgram {
  public:
    MetalShaderProgram() = default;
    ~MetalShaderProgram() override;

    id<MTLRenderPipelineState> getPipelineState() const;
    void bindUniforms(MetalRenderable* renderable);

  private:
    bool createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) override;

    MTLVertexDescriptor* makeVertexDescriptor(const Vector<VertexFormat>& vertexFormats);
    MTLVertexAttribute* findVertexAttribute(VertexSemantic semantic);
    bool makeRenderPipelineState();
    bool addShaderArguments(MTLRenderPipelineReflection* pipelineReflection);
    void addUniform(MTLArgument* argument, ShaderType type);
    void reserveUniformMemory();

    static constexpr MTLVertexFormat mapVertexFormatFrom(VertexAttributeType type);
    static constexpr UniformType mapUniformTypeFrom(MTLDataType dataType);
    static constexpr std::size_t mapUniformTypeSizeForm(UniformType uniformType);
    static constexpr const char* mapAttributeNameFrom(VertexSemantic semantic);
    static bool createShader(const std::string& source, id<MTLLibrary>& library, id<MTLFunction>& function);

    id<MTLLibrary> mVertexLibrary{nullptr};
    id<MTLLibrary> mFragmentLibrary{nullptr};
    id<MTLFunction> mVertexFunction{nullptr};
    id<MTLFunction> mFragmentFunction{nullptr};
    id<MTLRenderPipelineState> mPipelineState{nullptr};
    StreamBuffer mVertexUniformBuffer;
    StreamBuffer mFragmentUniformBuffer;
  };
}  // namespace GLaDOS

#endif

#endif