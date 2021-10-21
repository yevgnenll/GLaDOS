#include "D3DX12ShaderProgram.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/Uniform.h"
#include "math/Math.h"

#undef max
#undef min

namespace GLaDOS {
    Logger* D3DX12ShaderProgram::logger = LoggerRegistry::getInstance().makeAndGetLogger("D3DX12ShaderProgram");

    ComPtr<ID3D12RootSignature> D3DX12ShaderProgram::getRootSignature() const {
        return mRootSignature;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC D3DX12ShaderProgram::getPipelineDescriptor() const {
        return mPipelineDescriptor;
    }

    bool D3DX12ShaderProgram::createShaderProgram(const std::string& vertex, const std::string& fragment) {
        bool isVsValid = D3DX12ShaderProgram::createShader(vertex, "vs_5_0", mVertexFunction);
        bool isFsValid = D3DX12ShaderProgram::createShader(fragment, "ps_5_0", mFragmentFunction);
        mIsValid = isVsValid && isFsValid;

        if (!makePipelineDescriptor()) {
            mIsValid = false;
        }

        mVertexShaderCode = vertex;
        mFragmentShaderCode = fragment;

        return mIsValid;
    }

    bool D3DX12ShaderProgram::makePipelineDescriptor() {
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
        if (FAILED(D3DReflect(mVertexFunction->GetBufferPointer(), mVertexFunction->GetBufferSize(),
                              IID_ID3D12ShaderReflection, reinterpret_cast<void**>(vertexShaderReflection.GetAddressOf())))) {
            LOG_ERROR(logger, "Failed to reflect Vertex shader.");
            return false;
        }

        ComPtr<ID3D12ShaderReflection> fragmentShaderReflection;
        if (FAILED(D3DReflect(mFragmentFunction->GetBufferPointer(), mFragmentFunction->GetBufferSize(),
                              IID_ID3D12ShaderReflection, reinterpret_cast<void**>(fragmentShaderReflection.GetAddressOf())))) {
            LOG_ERROR(logger, "Failed to reflect Fragment shader.");
            return false;
        }

        if (!addShaderArguments(vertexShaderReflection, fragmentShaderReflection)) {
            return false;
        }

        createInputLayout(vertexShaderReflection);

        // 루트 서명의 유효성은 파이프라인 상태 객체를 생성할 때 검증된다.
        ZeroMemory(&mPipelineDescriptor, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
        mPipelineDescriptor.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
        mPipelineDescriptor.pRootSignature = mRootSignature.Get();
        mPipelineDescriptor.VS.pShaderBytecode = reinterpret_cast<BYTE*>(mVertexFunction->GetBufferPointer());
        mPipelineDescriptor.VS.BytecodeLength = mVertexFunction->GetBufferSize();
        mPipelineDescriptor.PS.pShaderBytecode = reinterpret_cast<BYTE*>(mFragmentFunction->GetBufferPointer());
        mPipelineDescriptor.PS.BytecodeLength = mFragmentFunction->GetBufferSize();
        mPipelineDescriptor.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        mPipelineDescriptor.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        mPipelineDescriptor.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        mPipelineDescriptor.SampleMask = UINT_MAX;
        mPipelineDescriptor.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        mPipelineDescriptor.NumRenderTargets = 1;
        mPipelineDescriptor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        mPipelineDescriptor.SampleDesc.Count = 1;
        mPipelineDescriptor.SampleDesc.Quality = 0;
        mPipelineDescriptor.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        ComPtr<ID3D12PipelineState> mPipelineState;
        if (FAILED(device->CreateGraphicsPipelineState(&mPipelineDescriptor, IID_PPV_ARGS(&mPipelineState)))) {
            LOG_ERROR(logger, "Failed to create graphics pipeline state.");
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
        if (FAILED(vertexShaderReflection->GetDesc(&vertexShaderDesc))) {
            LOG_ERROR(logger, "Failed to get vertex shader descriptor");
            return false;
        }

        D3D12_SHADER_DESC fragmentShaderDesc;
        if (FAILED(fragmentShaderReflection->GetDesc(&fragmentShaderDesc))) {
            LOG_ERROR(logger, "Failed to get fragment shader descriptor");
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
            LOG_ERROR(logger, "Failed to get constant buffer from ShaderReflection.");
            return;
        }

        D3D12_SHADER_BUFFER_DESC constantBufferDesc;
        if (FAILED(constantBuffer->GetDesc(&constantBufferDesc))) {
            LOG_ERROR(logger, "Failed to get constant buffer descriptor.");
            return;
        }

        for (unsigned int i = 0; i < constantBufferDesc.Variables; i++) {
            ID3D12ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(i);
            if (variable == nullptr) {
                LOG_ERROR(logger, "Failed to get shader reflection variable.");
                return;
            }

            D3D12_SHADER_VARIABLE_DESC variableDesc;
            if (FAILED(variable->GetDesc(&variableDesc))) {
                LOG_ERROR(logger, "Failed to get shader reflection variable descriptor.");
                return;
            }

            addUniformByType(variable->GetType(), variableDesc, type);
        }
    }

    void D3DX12ShaderProgram::addUniformByType(ID3D12ShaderReflectionType* variableType, D3D12_SHADER_VARIABLE_DESC variableDesc, ShaderType type) {
        if (variableType == nullptr) {
            LOG_ERROR(logger, "Failed to get shader reflection variable descriptor.");
            return;
        }

        D3D12_SHADER_TYPE_DESC variableTypeDesc;
        if (FAILED(variableType->GetDesc(&variableTypeDesc))) {
            LOG_ERROR(logger, "Failed to get shader reflection variable type descriptor.");
            return;
        }

        switch (variableTypeDesc.Class) {
//            case D3D_SVC_STRUCT:
//                for (unsigned int j = 0; j < variableTypeDesc.Members; j++) {
//                    addUniformByType(variableType->GetMemberTypeByIndex(j), variableDesc, type); // TODO: variableDesc 가 다른 offset, size를 보고 있음
//                }
//                break;
            case D3D_SVC_SCALAR:
            case D3D_SVC_VECTOR:
            case D3D_SVC_MATRIX_ROWS:  // row-major matrix
            case D3D_SVC_MATRIX_COLUMNS: {  // column-major matrix
                auto finder = mUniforms.find(variableDesc.Name);
                if (finder == mUniforms.end()) {
                    Uniform* uniform = NEW_T(Uniform);
                    uniform->mShaderType = type;
                    uniform->mUniformType = D3DX12ShaderProgram::mapUniformTypeFrom(variableTypeDesc.Type, variableTypeDesc.Class, variableTypeDesc.Rows, variableTypeDesc.Columns);
                    uniform->mName = variableDesc.Name;
                    uniform->mCount = variableTypeDesc.Elements != 0 ? variableTypeDesc.Elements : 1;
                    uniform->mOffset = variableDesc.StartOffset;
                    uniform->resize(uniform->mCount * variableDesc.Size);
                    mUniforms.try_emplace(uniform->mName, uniform);
                    LOG_TRACE(logger, "Uniform add -> [{0}]", uniform->toString());
                }
                break;
            }
            default:
                LOG_WARN(logger, "Not supported Shader variable type: {0}", variableTypeDesc.Name);
                break;
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
            if (FAILED(shaderReflection->GetResourceBindingDesc(i, &inputBindDesc))) {
                LOG_ERROR(logger, "Failed to get shader input bind descriptor.");
                return false;
            }

            switch (inputBindDesc.Type) {
                case D3D_SIT_CBUFFER:
                    numConstantBuffer++;
                    minConstantBufferBindPoint = Math::min(minConstantBufferBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature add -> cbuffer [{0}] Name: {1}, BindPoint: b{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
                    break;
                case D3D_SIT_TBUFFER:
                    numTextureBuffer++;
                    minTextureBufferBindPoint = Math::min(minTextureBufferBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature add -> tbuffer [{0}] Name: {1}, BindPoint: t{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
                    break;
                case D3D_SIT_SAMPLER:
                    numSampler++;
                    minSamplerBindPoint = Math::min(minSamplerBindPoint, inputBindDesc.BindPoint);
                    LOG_TRACE(logger, "RootSignature add -> sampler [{0}] Name: {1}, BindPoint: s{2}", i, inputBindDesc.Name, inputBindDesc.BindPoint);
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
            descRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, numTextureBuffer, minTextureBufferBindPoint);
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
        if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, blobWithRootSignature.GetAddressOf(), errorMessage.GetAddressOf()))) {
            if (errorMessage != nullptr) {
                LOG_ERROR(logger, "{0}", StringUtils::normalize(static_cast<const char*>(errorMessage->GetBufferPointer())));
            }
            return false;
        }

        if (FAILED(D3DX12Renderer::getInstance().getDevice()->CreateRootSignature(0, blobWithRootSignature->GetBufferPointer(), blobWithRootSignature->GetBufferSize(),
                                                                                  IID_PPV_ARGS(mRootSignature.GetAddressOf())))) {
            LOG_ERROR(logger, "Failed to create root signature.");
            return false;
        }

        return true;
    }

    void D3DX12ShaderProgram::createInputLayout(const ComPtr<ID3D12ShaderReflection>& shaderReflection) {
        D3D12_SHADER_DESC shaderDesc;
        if (FAILED(shaderReflection->GetDesc(&shaderDesc))) {
            LOG_ERROR(logger, "Failed to get shader descriptor.");
            return;
        }

        for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i) {
            D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
            if (FAILED(shaderReflection->GetInputParameterDesc(i, &paramDesc))) {
                LOG_ERROR(logger, "Failed to get input parameter descriptor.");
                return;
            }

            D3D12_INPUT_ELEMENT_DESC inputElementDesc;
            inputElementDesc.SemanticName = paramDesc.SemanticName;
            inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;

            if (paramDesc.Mask == 1) {
                switch (paramDesc.ComponentType) {
                    case D3D_REGISTER_COMPONENT_UINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
                        break;
                    case D3D_REGISTER_COMPONENT_SINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
                        break;
                    case D3D_REGISTER_COMPONENT_FLOAT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                        break;
                    default:
                        LOG_ERROR(logger, "Not supported component type.");
                        break;
                }
            } else if (paramDesc.Mask <= 3) {
                switch (paramDesc.ComponentType) {
                    case D3D_REGISTER_COMPONENT_UINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                        break;
                    case D3D_REGISTER_COMPONENT_SINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                        break;
                    case D3D_REGISTER_COMPONENT_FLOAT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                        break;
                    default:
                        LOG_ERROR(logger, "Not supported component type.");
                        break;
                }
            } else if (paramDesc.Mask <= 7) {
                switch (paramDesc.ComponentType) {
                    case D3D_REGISTER_COMPONENT_UINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                        break;
                    case D3D_REGISTER_COMPONENT_SINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                        break;
                    case D3D_REGISTER_COMPONENT_FLOAT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                        break;
                    default:
                        LOG_ERROR(logger, "Not supported component type.");
                        break;
                }
            } else if (paramDesc.Mask <= 15) {
                switch (paramDesc.ComponentType) {
                    case D3D_REGISTER_COMPONENT_UINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                        break;
                    case D3D_REGISTER_COMPONENT_SINT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                        break;
                    case D3D_REGISTER_COMPONENT_FLOAT32:
                        inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                        break;
                    default:
                        LOG_ERROR(logger, "Not supported component type.");
                        break;
                }
            }

            inputElementDesc.InputSlot = 0;
            inputElementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            inputElementDesc.InstanceDataStepRate = 0;

            LOG_TRACE(logger, "InputLayout add -> SemanticName: {0}, SemanticIndex: {1}", inputElementDesc.SemanticName, inputElementDesc.SemanticIndex);

            mInputLayout.emplace_back(inputElementDesc);
        }
    }

    bool D3DX12ShaderProgram::createShader(const std::string& source, const std::string& target, ComPtr<ID3DBlob>& function) {
        UINT compileFlags = 0;
#if defined(_DEBUG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ComPtr<ID3DBlob> errors;
        if (FAILED(D3DCompile(source.c_str(), source.size(), nullptr, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
                              "main0", target.c_str(), compileFlags, 0, function.GetAddressOf(), errors.GetAddressOf()))) {
            if (errors != nullptr) {
                LOG_ERROR(logger, static_cast<char*>(errors->GetBufferPointer()));
            }
            return false;
        }

        return true;
    }

    constexpr UniformType D3DX12ShaderProgram::mapUniformTypeFrom(D3D_SHADER_VARIABLE_TYPE dataType, D3D_SHADER_VARIABLE_CLASS dataClass, uint32_t rows, uint32_t columns) {
        switch (dataType) {
            case D3D_SVT_BOOL:
                return UniformType::Bool;
            case D3D_SVT_INT:
                return UniformType::Int;
            case D3D_SVT_UINT:
                return UniformType::UInt;
            case D3D_SVT_FLOAT: {
                switch (dataClass) {
                    case D3D_SVC_VECTOR:
                        switch (columns) {
                            case 2:
                                return UniformType::Vec2;
                            case 3:
                                return UniformType::Vec3;
                            default:
                                return UniformType::Vec4;
                        }
                    case D3D_SVC_MATRIX_ROWS:
                    case D3D_SVC_MATRIX_COLUMNS:
                        if (rows != columns) {
                            LOG_ERROR(logger, "Only support square matrix. rows:{0}, columns:{1}", columns, rows);
                            return UniformType::Unknown;
                        }

                        switch (rows) {
                            case 1:
                                return UniformType::Float;
                            case 2:
                                return UniformType::Mat2;
                            case 3:
                                return UniformType::Mat3;
                            default:
                                return UniformType::Mat4;
                        }
                    default:
                        return UniformType::Float;
                }
            }
            case D3D_SVT_TEXTURE:
                return UniformType::Texture;
            case D3D_SVT_SAMPLER:
                return UniformType::Sampler;
            default:
                LOG_ERROR(logger, "Unknown data type");
                break;
        }

        return UniformType::Unknown;
    }
}

#endif