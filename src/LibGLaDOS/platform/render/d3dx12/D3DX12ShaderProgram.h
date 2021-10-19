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
      public:
        D3DX12ShaderProgram() = default;
        ~D3DX12ShaderProgram() override;

      private:
        bool createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexBuffer* vertexBuffer) override;
        bool makePipelineDescriptor(const VertexBuffer* vertexBuffer);
        bool addShaderArguments(const ComPtr<ID3D12ShaderReflection>& vertexShaderReflection, const ComPtr<ID3D12ShaderReflection>& fragmentShaderReflection);
        void addUniform(ID3D12ShaderReflectionConstantBuffer* constantBuffer, ShaderType type);
        bool createRootSignature(const ComPtr<ID3D12ShaderReflection>& shaderReflection, D3D12_SHADER_DESC shaderDesc);
        bool createInputLayout(const VertexBuffer* vertexBuffer);

        static bool createShader(const std::string& source, const std::string& target, ComPtr<ID3DBlob> function);
        static Logger* logger;

        ComPtr<ID3DBlob> mVertexFunction;
        ComPtr<ID3DBlob> mFragmentFunction;
        ComPtr<ID3D12RootSignature> mRootSignature;
        Vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
    };
}

#endif

#endif  // GLADOS_D3DX12SHADERPROGRAM_H
