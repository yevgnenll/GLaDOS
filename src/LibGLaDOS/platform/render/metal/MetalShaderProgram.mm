#include "MetalShaderProgram.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderState.h"
#include "MetalRenderable.h"
#include "MetalTypes.h"
#include "platform/apple/CocoaPlatform.h"
#include "platform/render/Uniform.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/CommonTypes.h"

namespace GLaDOS {
    Logger* MetalShaderProgram::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalShaderProgram");
    
    MetalShaderProgram::~MetalShaderProgram() {
        if (mIsValid) {
            [mVertexFunction release];
            [mFragmentFunction release];
            [mPipelineDescriptor release];
        }
    }

    MTLRenderPipelineDescriptor* MetalShaderProgram::getPipelineDescriptor() const {
        return mPipelineDescriptor;
    }

    void MetalShaderProgram::bindUniforms(MetalRenderable* renderable) {
        // copy uniform buffer data
        for (const auto& [key, uniform] : mUniforms) {
            if (uniform->isTextureType()) {
                continue;
            }

            void* data = uniform->pointer();
            if (data == nullptr) {
                LOG_ERROR(logger, "Uniform [offset({0})] in {1} should not be null", uniform->mOffset, uniform->mShaderType.toString());
                continue;
            }

            if (uniform->isUniformType()) {
                Blob* buffer = nullptr;
                if (uniform->mShaderType == ShaderType::VertexShader) {
                    buffer = &mVertexUniformBuffer;
                } else if (uniform->mShaderType == ShaderType::FragmentShader) {
                    buffer = &mFragmentUniformBuffer;
                } else {
                    continue;
                }
                std::memcpy(buffer->offsetOf(uniform->mOffset), data, uniform->size());
            }
        }

        // bind uniform buffer
        id<MTLRenderCommandEncoder> commandEncoder = MetalRenderer::getInstance().getCommandEncoder();
        if (!mVertexUniformBuffer.isEmpty()) {
            [commandEncoder setVertexBytes:mVertexUniformBuffer.pointer() length:mVertexUniformBuffer.size() atIndex:0];
        }

        if (!mFragmentUniformBuffer.isEmpty()) {
            [commandEncoder setFragmentBytes:mFragmentUniformBuffer.pointer() length:mFragmentUniformBuffer.size() atIndex:0];
        }

        [commandEncoder setDepthStencilState:metalDepthStencilState()];

        RasterizerDescription rasterizerDesc = metalRasterizerState()->mRasterizerDescription;
        [commandEncoder setTriangleFillMode:MetalTypes::fillModeToMetal(rasterizerDesc.mFillMode)];
        [commandEncoder setFrontFacingWinding:MetalTypes::windingModeToMetal(rasterizerDesc.mWindingMode)];
        if (rasterizerDesc.mFillMode == FillMode::Lines) {
            [commandEncoder setCullMode:MTLCullModeNone];  // Wireframe rendering should be no cull.
        } else {
            [commandEncoder setCullMode:MetalTypes::cullModeToMetal(rasterizerDesc.mCullMode)];
        }

        [commandEncoder setDepthBias:rasterizerDesc.mDepthBias slopeScale:rasterizerDesc.mSlopeScaleDepthBias clamp:rasterizerDesc.mDepthBiasClamp];

        // [commandEncoder setStencilFrontReferenceValue:1 backReferenceValue:1];
    }

    id<MTLDepthStencilState> MetalShaderProgram::metalDepthStencilState() {
        return static_cast<MetalDepthStencilState*>(depthStencilState())->getMetalDepthStencilState();
    }

    MetalRasterizerState* MetalShaderProgram::metalRasterizerState() {
        return static_cast<MetalRasterizerState*>(rasterizerState());
    }

    bool MetalShaderProgram::createShaderProgram(const std::string& vertex, const std::string& fragment) {
        bool isVsValid = MetalShaderProgram::createShader(vertex, mVertexFunction);
        bool isFsValid = MetalShaderProgram::createShader(fragment, mFragmentFunction);
        mIsValid = isVsValid && isFsValid;

        if (!makePipelineDescriptor()) {
            mIsValid = false;
        }

        mVertexShaderCode = vertex;
        mFragmentShaderCode = fragment;

        return mIsValid;
    }

    bool MetalShaderProgram::makePipelineDescriptor() {
        if (!mIsValid) {
            LOG_ERROR(logger, "Invalid Metal shader program");
            return false;
        }

        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state");
            return false;
        }

        mPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        [mPipelineDescriptor setVertexFunction:mVertexFunction];
        [mPipelineDescriptor setFragmentFunction:mFragmentFunction];

        MTLRenderPipelineColorAttachmentDescriptor* colorAttachmentDescriptor = mPipelineDescriptor.colorAttachments[0];
        if (colorAttachmentDescriptor == nil) {
            LOG_ERROR(logger, "Invalid ColorAttachment state");
            return false;
        }
        colorAttachmentDescriptor.pixelFormat = [MetalRenderer::getInstance().getMetalLayer() pixelFormat];
        mPipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        mPipelineDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;

        MTLVertexDescriptor* vertexDescriptor = makeVertexDescriptor(mVertexFunction.vertexAttributes);;
        [mPipelineDescriptor setVertexDescriptor:vertexDescriptor];

        NSError* stateError;
        MTLPipelineOption options = MTLPipelineOptionArgumentInfo | MTLPipelineOptionBufferTypeInfo;
        MTLRenderPipelineReflection* pipelineReflection;
        id<MTLRenderPipelineState> pipelineState = [device newRenderPipelineStateWithDescriptor:mPipelineDescriptor options:options reflection:&pipelineReflection error:&stateError];
        if (stateError != nil) {
            LOG_ERROR(logger, "RenderPipelineState build error occurred!: \n{0}", [[stateError description] UTF8String]);
            return false;
        }

        if (!addShaderArguments(pipelineReflection)) {
            return false;
        }

        [pipelineState release];

        return true;
    }

    MTLVertexDescriptor* MetalShaderProgram::makeVertexDescriptor(NSArray<MTLVertexAttribute*>* vertexAttributes) {
        MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor new] autorelease];

        std::size_t bufferIndex = 1;  // bufferIndex 0번은 유니폼으로 사용됨
        std::size_t offset = 0;
        for (MTLVertexAttribute* attribute in vertexAttributes) {
            if (attribute != nullptr) {
                std::size_t attributeIndex = attribute.attributeIndex;
                MTLVertexFormat vertexFormat = MetalTypes::metalDataTypeToVertexFormat(attribute.attributeType);
                vertexDescriptor.attributes[attributeIndex].format = vertexFormat;
                vertexDescriptor.attributes[attributeIndex].bufferIndex = bufferIndex;
                vertexDescriptor.attributes[attributeIndex].offset = offset;

                offset += align4(MetalTypes::metalVertexFormatToSize(vertexFormat));
            }
        }

        if (offset != 0) {
            vertexDescriptor.layouts[bufferIndex].stride = offset;
        }

        return vertexDescriptor;
    }

    bool MetalShaderProgram::addShaderArguments(MTLRenderPipelineReflection* pipelineReflection) {
        if (pipelineReflection == nil) {
            LOG_ERROR(logger, "Invalid RenderPipelineReflection state");
            return false;
        }

        mUniforms.clear();
        for (MTLArgument* arg in pipelineReflection.vertexArguments) {
            parseUniform(arg, ShaderType::VertexShader);
        }

        for (MTLArgument* arg in pipelineReflection.fragmentArguments) {
            parseUniform(arg, ShaderType::FragmentShader);
        }

        reserveUniformMemory();

        return true;
    }

    void MetalShaderProgram::parseUniform(MTLArgument* argument, ShaderType type) {
        switch (argument.type) {
            case MTLArgumentTypeBuffer: {
                MTLStructType* structType = argument.bufferStructType;
                if (structType != nullptr) {
                    for (MTLStructMember* member in structType.members) {
                        std::string name = [member.name UTF8String];
                        Uniform* uniform = NEW_T(Uniform);
                        uniform->mShaderType = type;
                        uniform->mUniformType = MetalTypes::metalDataTypeToUniformType(member.dataType);
                        uniform->mName = name;
                        uniform->mCount = member.arrayType != nullptr ? member.arrayType.arrayLength : 1;
                        uniform->mOffset = member.offset;
                        uniform->resize(uniform->mCount * CommonTypes::uniformTypeToSize(uniform->mUniformType));

                        if (!addUniform(name, uniform)) {
                            DELETE_T(uniform, Uniform);
                        }
                    }
                }
                break;
            }
            case MTLArgumentTypeTexture: {
                std::string name = [argument.name UTF8String];
                Uniform* uniform = NEW_T(Uniform);
                uniform->mShaderType = type;
                uniform->mUniformType = MetalTypes::metalDataTypeToUniformType(MTLDataTypeTexture);
                uniform->mName = name;
                uniform->mCount = 1;
                uniform->mOffset = argument.index;

                if (!addUniform(name, uniform)) {
                    DELETE_T(uniform, Uniform);
                }
                break;
            }
            default:
                LOG_WARN(logger, "Not supported variable type: {0}", argument.type);
                break;
        }
    }

    bool MetalShaderProgram::createShader(const std::string& source, id<MTLFunction>& function) {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state");
            return false;
        }

        NSString* shaderSource = CocoaPlatform::toString(source);
        MTLCompileOptions* compileOptions = [MTLCompileOptions new];
        compileOptions.languageVersion = MTLLanguageVersion1_1;
        NSError* compileError;
        id<MTLLibrary> library = [device newLibraryWithSource:shaderSource options:compileOptions error:&compileError];
        [compileOptions release];

        if (compileError != nil) {
            NSString* errorMessage = [NSString stringWithFormat:@"%@", compileError];
            LOG_ERROR(logger, [errorMessage UTF8String]);
            [library release];
            return false;
        }

        function = [library newFunctionWithName:@"main0"];
        [library release];
        if (function == nil) {
            LOG_ERROR(logger, "Invalid function name main0");
            return false;
        }

        return true;
    }
}

#endif