#ifndef GLADOS_D3DX12SHADERPROGRAM_H
#define GLADOS_D3DX12SHADERPROGRAM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/ShaderProgram.h"
#include "platform/render/d3dx12/D3DX12Renderer.h"

namespace GLaDOS {
    class Logger;
    class D3DX12ShaderProgram : public ShaderProgram {
      public:
        D3DX12ShaderProgram() = default;
        ~D3DX12ShaderProgram() override;

      private:
        bool createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexBuffer* vertexBuffer) override;
        static bool createShader(const std::string& source, ComPtr<ID3DBlob> function);
        static Logger* logger;

        ComPtr<ID3DBlob> mVertexFunction;
        ComPtr<ID3DBlob> mFragmentFunction;
    };
}

#endif

#endif  // GLADOS_D3DX12SHADERPROGRAM_H
