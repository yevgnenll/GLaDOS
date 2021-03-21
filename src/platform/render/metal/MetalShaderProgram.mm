#include "MetalShaderProgram.h"

#ifdef PLATFORM_MACOS

#include <cstring>
#include "MetalRenderState.h"
#include "MetalRenderable.h"
#include "platform/apple/CocoaPlatform.h"
#include "platform/render/Uniform.h"
#include "platform/render/VertexData.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
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

  void MetalShaderProgram::bindUniforms(MetalRenderable* _renderable) {
    // copy uniform buffer data
    for (const auto& [key, uniform] : mUniforms) {
      void* data = uniform->pointer();
      if ((data != nullptr) && uniform->isUniformType()) {
        StreamBuffer* buffer = nullptr;
        if (uniform->mShaderType == ShaderType::VertexShader) {
          buffer = &mVertexUniformBuffer;
        } else if (uniform->mShaderType == ShaderType::FragmentShader) {
          buffer = &mFragmentUniformBuffer;
        } else {
          continue;
        }
        std::memcpy(buffer->offsetOf(uniform->mOffset), data, uniform->size());  // TODO: reimplement to c++ std::copy
      }
    }

    // bind uniform buffer
    id<MTLRenderCommandEncoder> commandEncoder = MetalRenderer::getInstance()->getCommandEncoder();
    if (!mVertexUniformBuffer.isEmpty()) {
      [commandEncoder setVertexBytes:mVertexUniformBuffer.pointer() length:mVertexUniformBuffer.size() atIndex:0];
    }

    if (!mFragmentUniformBuffer.isEmpty()) {
      [commandEncoder setFragmentBytes:mFragmentUniformBuffer.pointer() length:mFragmentUniformBuffer.size() atIndex:0];
    }

    // set the metal depth stencil state
    // TODO: 옮기기
    if (mDepthStencilState != nullptr) {
      [commandEncoder setDepthStencilState:metalDepthStencilState()];
      [commandEncoder setFrontFacingWinding:MTLWindingCounterClockwise];
      if (MetalRenderer::getInstance()->getFillMode() == FillMode::Lines) {
        [commandEncoder setCullMode:MTLCullModeNone];
      } else {
        // follow setting
        [commandEncoder setCullMode:MTLCullModeBack];
      }

      //      [commandEncoder setStencilFrontReferenceValue:1 backReferenceValue:1];
    }
  }

  MTLVertexDescriptor* MetalShaderProgram::makeVertexDescriptor(const Vector<VertexFormat*>& vertexFormats) {
    MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor new] autorelease];

    std::size_t bufferIndex = 1;  // bufferIndex 0번은 유니폼으로 사용됨
    std::size_t offset = 0;
    for (const auto& format : vertexFormats) {
      MTLVertexAttribute* attribute = findVertexAttribute(format->semantic());
      if (attribute != nullptr) {
        std::size_t attributeIndex = attribute.attributeIndex;
        vertexDescriptor.attributes[attributeIndex].format = MetalShaderProgram::mapVertexFormatFrom(format->type());
        vertexDescriptor.attributes[attributeIndex].bufferIndex = bufferIndex;
        vertexDescriptor.attributes[attributeIndex].offset = offset;
      }

      offset += format->sizeAlign4();
    }

    if (offset != 0) {
      vertexDescriptor.layouts[bufferIndex].stride = offset;
    }

    return vertexDescriptor;
  }

  id<MTLDepthStencilState> MetalShaderProgram::metalDepthStencilState() {
    return static_cast<MetalDepthStencilState*>(depthStencilState())->getMetalDepthStencilState();
  }

  bool MetalShaderProgram::createShaderProgram(const std::string& vertex, const std::string& fragment, const VertexData* vertexData) {
    bool isVsValid = MetalShaderProgram::createShader(vertex, mVertexFunction);
    bool isFsValid = MetalShaderProgram::createShader(fragment, mFragmentFunction);
    mIsValid = isVsValid && isFsValid;

    if (!makePipelineDescriptor(vertexData)) {
      mIsValid = false;
      return mIsValid;
    }

    mVertexShaderCode = vertex;
    mFragmentShaderCode = fragment;

    return mIsValid;
  }

  MTLVertexAttribute* MetalShaderProgram::findVertexAttribute(VertexSemantic semantic) {
    if (!mIsValid) {
      return nullptr;
    }

    std::string attributeName = MetalShaderProgram::mapAttributeNameFrom(semantic);
    for (MTLVertexAttribute* attr in mVertexFunction.vertexAttributes) {
      if ([attr.name UTF8String] == attributeName) {
        return attr;
      }
    }

    return nullptr;
  }

  bool MetalShaderProgram::makePipelineDescriptor(const VertexData* vertexData) {
    if (!mIsValid) {
      LOG_ERROR("Invalid Metal shader program");
      return false;
    }

    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nil) {
      LOG_ERROR("Invalid Metal device state");
      return false;
    }

    mPipelineDescriptor = [MTLRenderPipelineDescriptor new];
    [mPipelineDescriptor setVertexFunction:mVertexFunction];
    [mPipelineDescriptor setFragmentFunction:mFragmentFunction];

    MTLRenderPipelineColorAttachmentDescriptor* colorAttachmentDescriptor = mPipelineDescriptor.colorAttachments[0];
    if (colorAttachmentDescriptor == nil) {
      LOG_ERROR("Invalid ColorAttachment state");
      return false;
    }
    colorAttachmentDescriptor.pixelFormat = [MetalRenderer::getInstance()->getMetalLayer() pixelFormat];
    mPipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    mPipelineDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;

    MTLVertexDescriptor* vertexDescriptor = makeVertexDescriptor(vertexData->getVertexFormats());
    [mPipelineDescriptor setVertexDescriptor:vertexDescriptor];

    NSError* stateError;
    MTLPipelineOption options = MTLPipelineOptionArgumentInfo | MTLPipelineOptionBufferTypeInfo;
    MTLRenderPipelineReflection* pipelineReflection;
    id<MTLRenderPipelineState> pipelineState = [device newRenderPipelineStateWithDescriptor:mPipelineDescriptor options:options reflection:&pipelineReflection error:&stateError];
    if (stateError != nil) {
      LOG_ERROR("RenderPipelineState build error occurred!: \n{0}", [[stateError description] UTF8String]);
      return false;
    }

    if (!addShaderArguments(pipelineReflection)) {
      return false;
    }

    [pipelineState release];

    return true;
  }

  bool MetalShaderProgram::addShaderArguments(MTLRenderPipelineReflection* pipelineReflection) {
    if (pipelineReflection == nil) {
      LOG_ERROR("Invalid RenderPipelineReflection state");
      return false;
    }

    mUniforms.clear();
    for (MTLArgument* arg in pipelineReflection.vertexArguments) {
      addUniform(arg, ShaderType::VertexShader);
    }

    for (MTLArgument* arg in pipelineReflection.fragmentArguments) {
      addUniform(arg, ShaderType::FragmentShader);
    }

    reserveUniformMemory();

    return true;
  }

  void MetalShaderProgram::addUniform(MTLArgument* argument, ShaderType type) {
    switch (argument.type) {
      case MTLArgumentTypeBuffer: {
        MTLStructType* structType = argument.bufferStructType;
        if (structType != nullptr) {
          for (MTLStructMember* member in structType.members) {
            Uniform* uniform = NEW_T(Uniform);
            uniform->mShaderType = type;
            uniform->mUniformType = MetalShaderProgram::mapUniformTypeFrom(member.dataType);
            uniform->mName = [member.name UTF8String];
            uniform->mCount = member.arrayType != nullptr ? member.arrayType.arrayLength : 1;
            uniform->mOffset = member.offset;
            uniform->resize(uniform->mCount * MetalShaderProgram::mapUniformTypeSizeForm(uniform->mUniformType));
            mUniforms.try_emplace(uniform->mName, uniform);
            LOG_TRACE("Uniform add -> [{0}]", uniform->toString());
          }
        }
        break;
      }
      case MTLArgumentTypeTexture: {
        Uniform* uniform = NEW_T(Uniform);
        uniform->mShaderType = type;
        uniform->mUniformType = MetalShaderProgram::mapUniformTypeFrom(MTLDataTypeTexture);
        uniform->mName = [argument.name UTF8String];
        uniform->mCount = 1;
        uniform->mOffset = argument.index;
        mUniforms.try_emplace(uniform->mName, uniform);
        LOG_TRACE("Uniform add -> [{0}]", uniform->toString());
        break;
      }
    }
  }

  void MetalShaderProgram::reserveUniformMemory() {
    // 모든 유니폼객체에 대해 MTLBuffer를 생성하지 않고 바이트 스트림으로 셰이더에 전달하기 위해 메모리 배열을 위한 공간을 만든다.
    mVertexUniformBuffer.clear();
    mFragmentUniformBuffer.clear();

    std::size_t vertexUniformSize = 0;
    std::size_t fragmentUniformSize = 0;
    for (const auto& [key, uniform] : mUniforms) {
      if (!uniform->isUniformType()) {
        continue;
      }

      switch (uniform->mShaderType) {
        case ShaderType::VertexShader:
          vertexUniformSize += (uniform->mOffset + uniform->size());
          break;
        case ShaderType::FragmentShader:
          fragmentUniformSize += (uniform->mOffset + uniform->size());
          break;
        default:
          LOG_WARN("Not supported type yet!");
          break;
      }
    }

    mVertexUniformBuffer.resize(vertexUniformSize);
    mFragmentUniformBuffer.resize(fragmentUniformSize);
  }

  constexpr MTLVertexFormat MetalShaderProgram::mapVertexFormatFrom(VertexAttributeType type) {
    switch (type) {
      case VertexAttributeType::Float:
        return MTLVertexFormatFloat;
      case VertexAttributeType::Float2:
        return MTLVertexFormatFloat2;
      case VertexAttributeType::Float3:
        return MTLVertexFormatFloat3;
      case VertexAttributeType::Float4:
        return MTLVertexFormatFloat4;
      case VertexAttributeType::Half:
        return MTLVertexFormatHalf;
      case VertexAttributeType::Half2:
        return MTLVertexFormatHalf2;
      case VertexAttributeType::Half3:
        return MTLVertexFormatHalf3;
      case VertexAttributeType::Half4:
        return MTLVertexFormatHalf4;
      case VertexAttributeType::Int:
        return MTLVertexFormatInt;
      case VertexAttributeType::Int2:
        return MTLVertexFormatInt2;
      case VertexAttributeType::Int3:
        return MTLVertexFormatInt3;
      case VertexAttributeType::Int4:
        return MTLVertexFormatInt4;
      case VertexAttributeType::Byte:
        return MTLVertexFormatChar;
      case VertexAttributeType::Byte2:
        return MTLVertexFormatChar2;
      case VertexAttributeType::Byte3:
        return MTLVertexFormatChar3;
      case VertexAttributeType::Byte4:
        return MTLVertexFormatChar4;
      case VertexAttributeType::UByte:
        return MTLVertexFormatUChar;
      case VertexAttributeType::UByte2:
        return MTLVertexFormatUChar2;
      case VertexAttributeType::UByte3:
        return MTLVertexFormatUChar3;
      case VertexAttributeType::UByte4:
        return MTLVertexFormatUChar4;
      case VertexAttributeType::UByteNorm:
        return MTLVertexFormatUCharNormalized;
      case VertexAttributeType::UByte4Norm:
        return MTLVertexFormatUChar4Normalized;
      case VertexAttributeType::Short2:
        return MTLVertexFormatShort2;
      case VertexAttributeType::Short4:
        return MTLVertexFormatShort4;
      case VertexAttributeType::UShort2:
        return MTLVertexFormatUShort2;
      case VertexAttributeType::UShort2Norm:
        return MTLVertexFormatUShort2Normalized;
      case VertexAttributeType::Uint:
        return MTLVertexFormatUInt;
      case VertexAttributeType::UShortNorm:
        return MTLVertexFormatUShortNormalized;
      case VertexAttributeType::UShort4Norm:
        return MTLVertexFormatUShort4Normalized;
      default:
        LOG_ERROR("Unknown vertex attribute type");
        break;
    }

    return MTLVertexFormatInvalid;
  }

  constexpr UniformType MetalShaderProgram::mapUniformTypeFrom(MTLDataType dataType) {
    switch (dataType) {
      case MTLDataTypeBool:
        return UniformType::Bool;
      case MTLDataTypeInt:
        return UniformType::Int;
      case MTLDataTypeUInt:
        return UniformType::UInt;
      case MTLDataTypeFloat:
        return UniformType::Float;
      case MTLDataTypeFloat2:
        return UniformType::Vec2;
      case MTLDataTypeFloat3:
        return UniformType::Vec3;
      case MTLDataTypeFloat4:
        return UniformType::Vec4;
      case MTLDataTypeFloat4x4:
        return UniformType::Mat4;
      case MTLDataTypeTexture:
        return UniformType::Texture;
      case MTLDataTypeSampler:
        return UniformType::Sampler;
      default:
        LOG_ERROR("Unknown data type");
        break;
    }

    return UniformType::Unknown;
  }

  constexpr std::size_t MetalShaderProgram::mapUniformTypeSizeForm(UniformType uniformType) {
    switch (uniformType) {
      case UniformType::Bool:
        return 1;
      case UniformType::Int:
        return 4;
      case UniformType::UInt:
        return 4;
      case UniformType::Float:
        return 4;
      case UniformType::Vec2:
        return 8;
      case UniformType::Vec3:
        return 12;
      case UniformType::Vec4:
        return 16;
      case UniformType::Mat4:
        return 64;
      case UniformType::Texture:
        return 4;
      case UniformType::Sampler:
        return 4;
      default:
        LOG_ERROR("Unknown uniform type");
        break;
    }

    return 0;
  }

  constexpr const char* MetalShaderProgram::mapAttributeNameFrom(VertexSemantic semantic) {
    switch (semantic) {
      case VertexSemantic::Position:
        return "_position";
      case VertexSemantic::Normal:
        return "_normal";
      case VertexSemantic::Color:
        return "_color";
      case VertexSemantic::TexCoord0:
        return "_texCoord0";
      case VertexSemantic::TexCoord1:
        return "_texCoord1";
      case VertexSemantic::TexCoord2:
        return "_texCoord2";
      case VertexSemantic::TexCoord3:
        return "_texCoord3";
      case VertexSemantic::TexCoord4:
        return "_texCoord4";
      case VertexSemantic::TexCoord5:
        return "_texCoord5";
      case VertexSemantic::TexCoord6:
        return "_texCoord6";
      case VertexSemantic::TexCoord7:
        return "_texCoord7";
      case VertexSemantic::Tangent:
        return "_tangent";
      case VertexSemantic::BiTangent:
        return "_biTangent";
      default:
        LOG_WARN("Not supported vertex semantic type");
        break;
    }

    return "";
  }

  bool MetalShaderProgram::createShader(const std::string& source, id<MTLFunction>& function) {
    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nil) {
      LOG_ERROR("Invalid Metal device state");
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
      LOG_ERROR([errorMessage UTF8String]);
      [library release];
      return false;
    }

    function = [library newFunctionWithName:@"main0"];
    [library release];
    if (function == nil) {
      LOG_ERROR("Invalid function name main0");
      return false;
    }

    return true;
  }
}

#endif