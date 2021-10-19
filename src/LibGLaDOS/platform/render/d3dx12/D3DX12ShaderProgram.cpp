#include "D3DX12ShaderProgram.h"

#ifdef PLATFORM_WINDOW

namespace GLaDOS {
    D3DX12ShaderProgram::~D3DX12ShaderProgram() {

    }

    bool D3DX12ShaderProgram::createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexBuffer* vertexBuffer) {
        bool isVsValid = D3DX12ShaderProgram::createShader(vertex, mVertexFunction);
        bool isFsValid = D3DX12ShaderProgram::createShader(fragment, mFragmentFunction);
        mIsValid = isVsValid && isFsValid;

        mVertexShaderCode = vertex;
        mFragmentShaderCode = fragment;

        return mIsValid;
    }

    bool D3DX12ShaderProgram::createShader(const std::string& source, ComPtr<ID3DBlob> function) {
        UINT compileFlags = 0;
#if defined(_DEBUG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ComPtr<ID3DBlob> errors;
        HRESULT hresult = D3DCompile(source.c_str(), source.size(), nullptr, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                     "main0", "vs_5_0", compileFlags, 0, function.GetAddressOf(), errors.GetAddressOf());
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        if (errors != nullptr) {
            LOG_ERROR(logger, static_cast<char*>(errors->GetBufferPointer()));
            return false;
        }

        return true;
    }
}

#endif