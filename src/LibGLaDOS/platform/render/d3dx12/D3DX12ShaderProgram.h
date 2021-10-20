#ifndef GLADOS_D3DX12SHADERPROGRAM_H
#define GLADOS_D3DX12SHADERPROGRAM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/ShaderProgram.h"
#include "platform/render/d3dx12/D3DX12Renderer.h"
#include "utils/Stl.h"

namespace GLaDOS {
    class Logger;
    class D3DX12ShaderProgram : public ShaderProgram {
        friend class D3DX12Renderer;

      public:
        D3DX12ShaderProgram() = default;
        ~D3DX12ShaderProgram() override = default;

        ComPtr<ID3D12RootSignature> getRootSignature() const;

      private:
        bool createShaderProgram(const std::string& vertex, const std::string& fragment) override;
        bool makePipelineDescriptor();
        bool addShaderArguments(const ComPtr<ID3D12ShaderReflection>& vertexShaderReflection, const ComPtr<ID3D12ShaderReflection>& fragmentShaderReflection);
        void addUniform(ID3D12ShaderReflectionConstantBuffer* constantBuffer, ShaderType type);
        void addUniformByType(ID3D12ShaderReflectionType* variableType, D3D12_SHADER_VARIABLE_DESC variableDesc, ShaderType type);
        bool createRootSignature(const ComPtr<ID3D12ShaderReflection>& shaderReflection, D3D12_SHADER_DESC shaderDesc);
        void createInputLayout(const ComPtr<ID3D12ShaderReflection>& shaderReflection);

        static bool createShader(const std::string& source, const std::string& target, ComPtr<ID3DBlob>& function);
        static constexpr UniformType mapUniformTypeFrom(D3D_SHADER_VARIABLE_TYPE dataType, D3D_SHADER_VARIABLE_CLASS dataClass, uint32_t rows, uint32_t columns);
        static Logger* logger;

        ComPtr<ID3DBlob> mVertexFunction;
        ComPtr<ID3DBlob> mFragmentFunction;
        ComPtr<ID3D12RootSignature> mRootSignature;
        Vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
        D3D12_GRAPHICS_PIPELINE_STATE_DESC mPipelineDescriptor{};
    };
}

#endif

#endif  // GLADOS_D3DX12SHADERPROGRAM_H
