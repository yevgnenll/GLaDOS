#include "D3DX12ShaderProgram.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/Uniform.h"
#include "math/Math.h"

#undef max
#undef min

namespace GLaDOS {
    Logger* D3DX12ShaderProgram::logger = LoggerRegistry::getInstance().makeAndGetLogger("D3DX12ShaderProgram");
    D3DX12ShaderProgram::~D3DX12ShaderProgram() {

    }

    ComPtr<ID3D12RootSignature> D3DX12ShaderProgram::getRootSignature() const {
        return mRootSignature;
    }

    ComPtr<ID3D12PipelineState> D3DX12ShaderProgram::getPipelineState() const {
        return mPipelineState;
    }

    bool D3DX12ShaderProgram::createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexBuffer* vertexBuffer) {
        bool isVsValid = D3DX12ShaderProgram::createShader(vertex, "vs_5_0", mVertexFunction);
        bool isFsValid = D3DX12ShaderProgram::createShader(fragment, "ps_5_0", mFragmentFunction);
        mIsValid = isVsValid && isFsValid;

        if (!makePipelineDescriptor(vertexBuffer)) {
            mIsValid = false;
        }

        mVertexShaderCode = vertex;
        mFragmentShaderCode = fragment;

        return mIsValid;
    }

    bool D3DX12ShaderProgram::makePipelineDescriptor(const VertexBuffer* vertexBuffer) {
        if (!mIsValid) {
            LOG_ERROR(logger, "Invalid DirectX12 shader program");
            return false;
        }

        ComPtr<ID3D12Device> device = D3DX12Renderer::getInstance().getDevice();
        if (device == nullptr) {
            LOG_ERROR(logger, "Invalid DirectX12 device state");
            return false;
        }

        ComPtr<ID3D12ShaderReflection> vertexShaderReflection;
        HRESULT hresult = D3DReflect(mVertexFunction->GetBufferPointer(), mVertexFunction->GetBufferSize(),
                                     IID_ID3D12ShaderReflection, reinterpret_cast<void**>(vertexShaderReflection.GetAddressOf()));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        ComPtr<ID3D12ShaderReflection> fragmentShaderReflection;
        hresult = D3DReflect(mFragmentFunction->GetBufferPointer(), mFragmentFunction->GetBufferSize(),
                                     IID_ID3D12ShaderReflection, reinterpret_cast<void**>(fragmentShaderReflection.GetAddressOf()));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        if (!addShaderArguments(vertexShaderReflection, fragmentShaderReflection)) {
            return false;
        }

        createInputLayout(vertexBuffer);

        // 루트 서명의 유효성은 파이프라인 상태 객체를 생성할 때 검증된다.
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDescriptor;
        ZeroMemory(&pipelineDescriptor, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
        pipelineDescriptor.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
        pipelineDescriptor.pRootSignature = mRootSignature.Get();
        pipelineDescriptor.VS =
            {
                reinterpret_cast<BYTE*>(mVertexFunction->GetBufferPointer()),
                mVertexFunction->GetBufferSize()
            };
        pipelineDescriptor.PS =
            {
                reinterpret_cast<BYTE*>(mFragmentFunction->GetBufferPointer()),
                mFragmentFunction->GetBufferSize()
            };
        pipelineDescriptor.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        pipelineDescriptor.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        pipelineDescriptor.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        pipelineDescriptor.SampleMask = UINT_MAX;
        pipelineDescriptor.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pipelineDescriptor.NumRenderTargets = 1;
        pipelineDescriptor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        pipelineDescriptor.SampleDesc.Count = 1;
        pipelineDescriptor.SampleDesc.Quality = 0;
        pipelineDescriptor.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        hresult = device->CreateGraphicsPipelineState(&pipelineDescriptor, IID_PPV_ARGS(&mPipelineState));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        return true;
    }

    bool D3DX12ShaderProgram::addShaderArguments(const ComPtr<ID3D12ShaderReflection>& vertexShaderReflection, const ComPtr<ID3D12ShaderReflection>& fragmentShaderReflection) {
        if (vertexShaderReflection == nullptr) {
            LOG_ERROR(logger, "Invalid Vertex Shader Reflection");
            return false;
        }

        if (fragmentShaderReflection == nullptr) {
            LOG_ERROR(logger, "Invalid Fragment Shader Reflection");
            return false;
        }

        D3D12_SHADER_DESC vertexShaderDesc;
        HRESULT hresult = vertexShaderReflection->GetDesc(&vertexShaderDesc);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        D3D12_SHADER_DESC fragmentShaderDesc;
        hresult = fragmentShaderReflection->GetDesc(&fragmentShaderDesc);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        if (!createRootSignature(vertexShaderReflection, vertexShaderDesc)) {
            LOG_ERROR(logger, "Invalid Vertex Shader RootSignature");
            return false;
        }

        mUniforms.clear();
        for (unsigned int i = 0; i < vertexShaderDesc.ConstantBuffers; i++) {
            addUniform(vertexShaderReflection->GetConstantBufferByIndex(i), ShaderType::VertexShader);
        }

        for (unsigned int i = 0; i < fragmentShaderDesc.ConstantBuffers; i++) {
            addUniform(fragmentShaderReflection->GetConstantBufferByIndex(i), ShaderType::FragmentShader);
        }

        reserveUniformMemory();

        return true;
    }

    void D3DX12ShaderProgram::addUniform(ID3D12ShaderReflectionConstantBuffer* constantBuffer, ShaderType type) {
        if (constantBuffer == nullptr) {
            LOG_ERROR(logger, "DirectX12 Reflection error: constant buffer should not be nullptr.");
            return;
        }

        D3D12_SHADER_BUFFER_DESC constantBufferDesc;
        HRESULT hresult = constantBuffer->GetDesc(&constantBufferDesc);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return;
        }

        for (unsigned int i = 0; i < constantBufferDesc.Variables; i++) {
            ID3D12ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(i);
            if (variable == nullptr) {
                LOG_ERROR(logger, "DirectX12 Reflection error: variable should not be nullptr.");
                return;
            }

            D3D12_SHADER_VARIABLE_DESC variableDesc;
            hresult = variable->GetDesc(&variableDesc);
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
                return;
            }

            ID3D12ShaderReflectionType* variableType = variable->GetType();
            if (variableType == nullptr) {
                LOG_ERROR(logger, "DirectX12 Reflection error: variable type should not be nullptr.");
                return;
            }

            D3D12_SHADER_TYPE_DESC variableTypeDesc;
            hresult = variableType->GetDesc(&variableTypeDesc);
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
                return;
            }

            switch (variableTypeDesc.Class) {
                // TODO
                case D3D_SVC_STRUCT: {
                    Uniform* uniform = NEW_T(Uniform);
                    uniform->mShaderType = type;
                    //                    uniform->mUniformType = MetalShaderProgram::mapUniformTypeFrom(member.dataType);
                    uniform->mName = variableTypeDesc.Name;
                    //                    uniform->mCount = member.arrayType != nullptr ? member.arrayType.arrayLength : 1;
                    //                    uniform->mOffset = member.offset;
                    //                    uniform->resize(uniform->mCount * MetalShaderProgram::mapUniformTypeSizeForm(uniform->mUniformType));
                    mUniforms.try_emplace(uniform->mName, uniform);
                    LOG_TRACE(logger, "Uniform add -> [{0}]", uniform->toString());
                    break;
                }
                case D3D_SVC_SCALAR:
                    break;
                case D3D_SVC_VECTOR:
                    break;
                case D3D_SVC_MATRIX_ROWS:
                    break;
                case D3D_SVC_MATRIX_COLUMNS:
                    break;
                case D3D_SVC_OBJECT:
                    break;
                case D3D_SVC_INTERFACE_CLASS:
                    break;
                case D3D_SVC_INTERFACE_POINTER:
                    break;
                case D3D_SVC_FORCE_DWORD:
                    break;
            }
        }
    }

    bool D3DX12ShaderProgram::createRootSignature(const ComPtr<ID3D12ShaderReflection>& shaderReflection, D3D12_SHADER_DESC shaderDesc) {
        uint32_t numConstantBuffer = 0;
        uint32_t minConstantBufferBindPoint = 999;
        uint32_t numTextureBuffer = 0;
        uint32_t minTextureBufferBindPoint = 999;
        uint32_t numSampler = 0;
        uint32_t minSamplerBindPoint = 999;
        for (unsigned int i = 0; i < shaderDesc.BoundResources; i++) {
            D3D12_SHADER_INPUT_BIND_DESC inputBindDesc;
            HRESULT hresult = shaderReflection->GetResourceBindingDesc(i, &inputBindDesc);
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
                return false;
            }

            switch (inputBindDesc.Type) {
                case D3D_SIT_CBUFFER:
                    numConstantBuffer++;
                    minConstantBufferBindPoint = Math::min(minConstantBufferBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature cbuffer [{0}] name: {1}, registerPoint: b{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
                    break;
                case D3D_SIT_TBUFFER:
                    numTextureBuffer++;
                    minTextureBufferBindPoint = Math::min(minTextureBufferBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature tbuffer [{0}] name: {1}, registerPoint: t{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
                    break;
                case D3D_SIT_SAMPLER:
                    numSampler++;
                    minSamplerBindPoint = Math::min(minSamplerBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature sampler [{0}] name: {1}, registerPoint: s{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
                    break;
                default:
                    LOG_WARN(logger, "Not supported Resource binding type.");
                    break;
            }
        }

        // TODO: 상수 버퍼 사용 빈도에 따라 다른 루트 파라미터 테이블에 넣도록 수정 (성능 이슈)
        uint32_t numDescRange = 0;
        CD3DX12_DESCRIPTOR_RANGE descRange[3];
        if (numConstantBuffer != 0) {
            descRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, numConstantBuffer, minConstantBufferBindPoint);
            numDescRange++;
        }

        if (numTextureBuffer != 0) {
            descRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, numTextureBuffer, minTextureBufferBindPoint);
            numDescRange++;
        }

        if (numSampler != 0) {
            descRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, numSampler, minSamplerBindPoint);
            numDescRange++;
        }

        CD3DX12_ROOT_PARAMETER slotRootParameter[1];
        slotRootParameter[0].InitAsDescriptorTable(numDescRange, descRange, D3D12_SHADER_VISIBILITY_ALL);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr,
                                                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3DBlob> blobWithRootSignature;
        ComPtr<ID3DBlob> errorMessage;
        HRESULT hresult = D3D12SerializeRootSignature(
            &rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, blobWithRootSignature.GetAddressOf(), errorMessage.GetAddressOf());
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        if(errorMessage != nullptr) {
            LOG_ERROR(logger, "{0}", StringUtils::normalize(static_cast<const char*>(errorMessage->GetBufferPointer())));
            return false;
        }

        hresult = D3DX12Renderer::getInstance().getDevice()->CreateRootSignature(
            0, blobWithRootSignature->GetBufferPointer(), blobWithRootSignature->GetBufferSize(), IID_PPV_ARGS(mRootSignature.GetAddressOf()));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        return true;
    }

    void D3DX12ShaderProgram::createInputLayout(const VertexBuffer* vertexBuffer) {
        mInputLayout = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
    }

    bool D3DX12ShaderProgram::createShader(const std::string& source, const std::string& target, ComPtr<ID3DBlob>& function) {
        UINT compileFlags = 0;
#if defined(_DEBUG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ComPtr<ID3DBlob> errors;
        HRESULT hresult = D3DCompile(source.c_str(), source.size(), nullptr, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                     "main0", target.c_str(), compileFlags, 0, function.GetAddressOf(), errors.GetAddressOf());
        if (errors != nullptr) {
            LOG_ERROR(logger, static_cast<char*>(errors->GetBufferPointer()));
        }

        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", D3DX12Renderer::hresultToString(hresult));
            return false;
        }

        return true;
    }
}

#endif