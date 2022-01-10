#include "MetalTypes.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
    Logger* MetalTypes::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalTypes");

    MTLVertexFormat MetalTypes::vertexAttribTypeToMetal(VertexAttributeType type, bool normalized) {
        switch (type) {
            case VertexAttributeType::Float:        return MTLVertexFormatFloat;
            case VertexAttributeType::Float2:       return MTLVertexFormatFloat2;
            case VertexAttributeType::Float3:       return MTLVertexFormatFloat3;
            case VertexAttributeType::Float4:       return MTLVertexFormatFloat4;

            case VertexAttributeType::Half:         return MTLVertexFormatHalf;
            case VertexAttributeType::Half2:        return MTLVertexFormatHalf2;
            case VertexAttributeType::Half3:        return MTLVertexFormatHalf3;
            case VertexAttributeType::Half4:        return MTLVertexFormatHalf4;

            case VertexAttributeType::Int:          return MTLVertexFormatInt;
            case VertexAttributeType::Int2:         return MTLVertexFormatInt2;
            case VertexAttributeType::Int3:         return MTLVertexFormatInt3;
            case VertexAttributeType::Int4:         return MTLVertexFormatInt4;

            case VertexAttributeType::UInt:         return MTLVertexFormatUInt;
            case VertexAttributeType::UInt2:        return MTLVertexFormatUInt2;
            case VertexAttributeType::UInt3:        return MTLVertexFormatUInt3;
            case VertexAttributeType::UInt4:        return MTLVertexFormatUInt4;

            case VertexAttributeType::Byte:         return MTLVertexFormatChar;
            case VertexAttributeType::Byte2:        return MTLVertexFormatChar2;
            case VertexAttributeType::Byte3:        return MTLVertexFormatChar3;
            case VertexAttributeType::Byte4:        return MTLVertexFormatChar4;

            case VertexAttributeType::UByte:        return MTLVertexFormatUChar;
            case VertexAttributeType::UByte2:       return MTLVertexFormatUChar2;
            case VertexAttributeType::UByte3:       return MTLVertexFormatUChar3;
            case VertexAttributeType::UByte4:       return MTLVertexFormatUChar4;

            case VertexAttributeType::Short:        return normalized ? MTLVertexFormatShortNormalized  : MTLVertexFormatShort;
            case VertexAttributeType::Short2:       return normalized ? MTLVertexFormatShort2Normalized : MTLVertexFormatShort2;
            case VertexAttributeType::Short3:       return normalized ? MTLVertexFormatShort3Normalized : MTLVertexFormatShort3;
            case VertexAttributeType::Short4:       return normalized ? MTLVertexFormatShort4Normalized : MTLVertexFormatShort4;

            case VertexAttributeType::UShort:       return normalized ? MTLVertexFormatUShortNormalized  : MTLVertexFormatUShort;
            case VertexAttributeType::UShort2:      return normalized ? MTLVertexFormatUShort2Normalized : MTLVertexFormatUShort2;
            case VertexAttributeType::UShort3:      return normalized ? MTLVertexFormatUShort3Normalized : MTLVertexFormatUShort3;
            case VertexAttributeType::UShort4:      return normalized ? MTLVertexFormatUShort4Normalized : MTLVertexFormatUShort4;

            default:
                LOG_ERROR(logger, "Unknown vertex attribute type");
                break;
        }

        return MTLVertexFormatInvalid;
    }

    std::size_t MetalTypes::metalVertexFormatToSize(MTLVertexFormat vertexFormat) {
        const uint32_t component_32bit_byte_size = 4;
        const uint32_t component_16bit_byte_size = 2;

        switch (vertexFormat) {
            case MTLVertexFormatFloat:
            case MTLVertexFormatInt:
            case MTLVertexFormatUInt:
                return 1 * component_32bit_byte_size;

            case MTLVertexFormatFloat2:
            case MTLVertexFormatInt2:
            case MTLVertexFormatUInt2:
                return 2 * component_32bit_byte_size;

            case MTLVertexFormatFloat3:
            case MTLVertexFormatInt3:
            case MTLVertexFormatUInt3:
                return 3 * component_32bit_byte_size;

            case MTLVertexFormatFloat4:
            case MTLVertexFormatInt4:
            case MTLVertexFormatUInt4:
                return 4 * component_32bit_byte_size;

            case MTLVertexFormatHalf:
            case MTLVertexFormatShort:
            case MTLVertexFormatShortNormalized:
            case MTLVertexFormatUShort:
            case MTLVertexFormatUShortNormalized:
                return 1 * component_16bit_byte_size;

            case MTLVertexFormatHalf2:
            case MTLVertexFormatShort2:
            case MTLVertexFormatShort2Normalized:
            case MTLVertexFormatUShort2:
            case MTLVertexFormatUShort2Normalized:
                return 2 * component_16bit_byte_size;

            case MTLVertexFormatHalf3:
            case MTLVertexFormatShort3:
            case MTLVertexFormatShort3Normalized:
            case MTLVertexFormatUShort3:
            case MTLVertexFormatUShort3Normalized:
                return 3 * component_16bit_byte_size;

            case MTLVertexFormatHalf4:
            case MTLVertexFormatShort4:
            case MTLVertexFormatShort4Normalized:
            case MTLVertexFormatUShort4:
            case MTLVertexFormatUShort4Normalized:
                return 4 * component_16bit_byte_size;

            case MTLVertexFormatChar:
            case MTLVertexFormatCharNormalized:
            case MTLVertexFormatUChar:
            case MTLVertexFormatUCharNormalized:
                return 1;

            case MTLVertexFormatChar2:
            case MTLVertexFormatChar2Normalized:
            case MTLVertexFormatUChar2:
            case MTLVertexFormatUChar2Normalized:
                return 2;

            case MTLVertexFormatChar3:
            case MTLVertexFormatChar3Normalized:
            case MTLVertexFormatUChar3:
            case MTLVertexFormatUChar3Normalized:
                return 3;

            case MTLVertexFormatChar4:
            case MTLVertexFormatChar4Normalized:
            case MTLVertexFormatUChar4:
            case MTLVertexFormatUChar4Normalized:
                return 4;

            default:
                LOG_ERROR(logger, "Unknown vertex format");
                break;
        }

        return 0;
    }

    UniformType MetalTypes::metalDataTypeToUniformType(MTLDataType dataType) {
        switch (dataType) {
            case MTLDataTypeBool:      return UniformType::Bool;
            case MTLDataTypeInt:       return UniformType::Int;
            case MTLDataTypeInt2:      return UniformType::Int2;
            case MTLDataTypeInt3:      return UniformType::Int3;
            case MTLDataTypeInt4:      return UniformType::Int4;
            case MTLDataTypeUInt:      return UniformType::UInt;
            case MTLDataTypeUInt2:     return UniformType::UInt2;
            case MTLDataTypeUInt3:     return UniformType::UInt3;
            case MTLDataTypeUInt4:     return UniformType::UInt4;
            case MTLDataTypeFloat:     return UniformType::Float;
            case MTLDataTypeFloat2:    return UniformType::Vec2;
            case MTLDataTypeFloat3:    return UniformType::Vec3;
            case MTLDataTypeFloat4:    return UniformType::Vec4;
            case MTLDataTypeFloat2x2:  return UniformType::Mat2;
            case MTLDataTypeFloat3x3:  return UniformType::Mat3;
            case MTLDataTypeFloat4x4:  return UniformType::Mat4;
            case MTLDataTypeTexture:   return UniformType::Texture;
            case MTLDataTypeSampler:   return UniformType::Sampler;
            default:
                LOG_ERROR(logger, "Unknown data type");
                break;
        }

        return UniformType::Unknown;
    }

    MTLTriangleFillMode MetalTypes::fillModeToMetal(FillMode mode) {
        switch (mode) {
            case FillMode::Fill:   return MTLTriangleFillModeFill;
            case FillMode::Lines:  return MTLTriangleFillModeLines;
        }
    }

    MTLWinding MetalTypes::windingModeToMetal(WindingMode mode) {
        switch (mode) {
            case WindingMode::ClockWise:         return MTLWindingClockwise;
            case WindingMode::CounterClockWise:  return MTLWindingCounterClockwise;
        }
    }

    MTLCullMode MetalTypes::cullModeToMetal(CullMode mode) {
        switch (mode) {
            case CullMode::None:   return MTLCullModeNone;
            case CullMode::Front:  return MTLCullModeFront;
            case CullMode::Back:   return MTLCullModeBack;
        }
    }

    MTLVertexFormat MetalTypes::metalDataTypeToVertexFormat(MTLDataType dataType, bool normalized) {
        switch(dataType) {
            case MTLDataTypeFloat:      return MTLVertexFormatFloat;
            case MTLDataTypeFloat2:     return MTLVertexFormatFloat2;
            case MTLDataTypeFloat3:     return MTLVertexFormatFloat3;
            case MTLDataTypeFloat4:     return MTLVertexFormatFloat4;

            case MTLDataTypeHalf:       return MTLVertexFormatHalf;
            case MTLDataTypeHalf2:      return MTLVertexFormatHalf2;
            case MTLDataTypeHalf3:      return MTLVertexFormatHalf3;
            case MTLDataTypeHalf4:      return MTLVertexFormatHalf4;

            case MTLDataTypeInt:        return MTLVertexFormatInt;
            case MTLDataTypeInt2:       return MTLVertexFormatInt2;
            case MTLDataTypeInt3:       return MTLVertexFormatInt3;
            case MTLDataTypeInt4:       return MTLVertexFormatInt4;

            case MTLDataTypeUInt:       return MTLVertexFormatUInt;
            case MTLDataTypeUInt2:      return MTLVertexFormatUInt2;
            case MTLDataTypeUInt3:      return MTLVertexFormatUInt3;
            case MTLDataTypeUInt4:      return MTLVertexFormatUInt4;

            case MTLDataTypeShort:      return normalized ? MTLVertexFormatShortNormalized  : MTLVertexFormatShort;
            case MTLDataTypeShort2:     return normalized ? MTLVertexFormatShort2Normalized : MTLVertexFormatShort2;
            case MTLDataTypeShort3:     return normalized ? MTLVertexFormatShort3Normalized : MTLVertexFormatShort3;
            case MTLDataTypeShort4:     return normalized ? MTLVertexFormatShort4Normalized : MTLVertexFormatShort4;

            case MTLDataTypeUShort:     return normalized ? MTLVertexFormatUShortNormalized  : MTLVertexFormatUShort;
            case MTLDataTypeUShort2:    return normalized ? MTLVertexFormatUShort2Normalized : MTLVertexFormatUShort2;
            case MTLDataTypeUShort3:    return normalized ? MTLVertexFormatUShort3Normalized : MTLVertexFormatUShort3;
            case MTLDataTypeUShort4:    return normalized ? MTLVertexFormatUShort4Normalized : MTLVertexFormatUShort4;

            case MTLDataTypeChar:       return normalized ? MTLVertexFormatCharNormalized  : MTLVertexFormatChar;
            case MTLDataTypeChar2:      return normalized ? MTLVertexFormatChar2Normalized : MTLVertexFormatChar2;
            case MTLDataTypeChar3:      return normalized ? MTLVertexFormatChar3Normalized : MTLVertexFormatChar3;
            case MTLDataTypeChar4:      return normalized ? MTLVertexFormatChar4Normalized : MTLVertexFormatChar4;

            case MTLDataTypeUChar:      return normalized ? MTLVertexFormatUCharNormalized  : MTLVertexFormatUChar;
            case MTLDataTypeUChar2:     return normalized ? MTLVertexFormatUChar2Normalized : MTLVertexFormatUChar2;
            case MTLDataTypeUChar3:     return normalized ? MTLVertexFormatUChar3Normalized : MTLVertexFormatUChar3;
            case MTLDataTypeUChar4:     return normalized ? MTLVertexFormatUChar4Normalized : MTLVertexFormatUChar4;

            default:
                LOG_ERROR(logger, "Unknown vertex attribute type");
                break;
        }

        return MTLVertexFormatInvalid;
    }

    MTLPrimitiveType MetalTypes::primitiveTopologyToMetal(PrimitiveTopology primitiveTopology) {
        switch (primitiveTopology) {
            case PrimitiveTopology::Point:          return MTLPrimitiveTypePoint;
            case PrimitiveTopology::Line:           return MTLPrimitiveTypeLine;
            case PrimitiveTopology::LineStrip:      return MTLPrimitiveTypeLineStrip;
            case PrimitiveTopology::Triangle:       return MTLPrimitiveTypeTriangle;
            case PrimitiveTopology::TriangleStrip:  return MTLPrimitiveTypeTriangleStrip;
        }
    }

    MTLIndexType MetalTypes::sizeToMetalIndexType(std::size_t size) {
        switch (size) {
            case sizeof(uint32_t):  return MTLIndexTypeUInt32;
            case sizeof(uint16_t):  return MTLIndexTypeUInt16;
            default:                break;
        }

        return MTLIndexTypeUInt16;
    }

    MTLCompareFunction MetalTypes::comparisonFunctionToMetal(ComparisonFunction func) {
        switch (func) {
            case ComparisonFunction::Never:
                return MTLCompareFunctionNever;
            case ComparisonFunction::Always:
                return MTLCompareFunctionAlways;
            case ComparisonFunction::Less:
                return MTLCompareFunctionLess;
            case ComparisonFunction::LessEqual:
                return MTLCompareFunctionLessEqual;
            case ComparisonFunction::Equal:
                return MTLCompareFunctionEqual;
            case ComparisonFunction::NotEqual:
                return MTLCompareFunctionNotEqual;
            case ComparisonFunction::Greater:
                return MTLCompareFunctionGreater;
            case ComparisonFunction::GreaterEqual:
                return MTLCompareFunctionGreaterEqual;
            default:
                LOG_WARN(logger, "Unknown Depth comparison function! fallback to ComparisonFunction::Less");
                break;
        }

        return MTLCompareFunctionLess;
    }

    MTLStencilOperation MetalTypes::stencilOperatorToMetal(StencilOperator op) {
        switch (op) {
            case StencilOperator::Keep:
                return MTLStencilOperationKeep;
            case StencilOperator::Zero:
                return MTLStencilOperationZero;
            case StencilOperator::Replace:
                return MTLStencilOperationReplace;
            case StencilOperator::Increase:
                return MTLStencilOperationIncrementClamp;
            case StencilOperator::IncreaseWrap:
                return MTLStencilOperationIncrementWrap;
            case StencilOperator::Decrease:
                return MTLStencilOperationDecrementClamp;
            case StencilOperator::DecreaseWrap:
                return MTLStencilOperationDecrementWrap;
            case StencilOperator::Invert:
                return MTLStencilOperationInvert;
            default:
                LOG_WARN(logger, "Unknown Stencil operator! fallback to StencilOperator::Keep");
                break;
        }

        return MTLStencilOperationKeep;
    }

    MTLSamplerMinMagFilter MetalTypes::filterModeToMetalMinMagFilter(FilterMode mode) {
        switch (mode) {
            case FilterMode::Nearest:
                return MTLSamplerMinMagFilterNearest;
            case FilterMode::Bilinear:
                return MTLSamplerMinMagFilterLinear;
            default:
                LOG_WARN(logger, "Not recognized FilterMode in metal renderer fallback to Nearest");
        }

        return MTLSamplerMinMagFilterNearest;
    }

    MTLSamplerMipFilter MetalTypes::filterModeToMetalMipFilter(FilterMode mode) {
        switch (mode) {
            case FilterMode::None:
                return MTLSamplerMipFilterNotMipmapped;
            case FilterMode::Nearest:
                return MTLSamplerMipFilterNearest;
            case FilterMode::Bilinear:
                return MTLSamplerMipFilterLinear;
            default:
                LOG_WARN(logger, "Not recognized FilterMode in metal renderer fallback to Nearest");
                break;
        }

        return MTLSamplerMipFilterNotMipmapped;
    }

    MTLSamplerAddressMode MetalTypes::wrapModeToMetalAddressMode(WrapMode mode) {
        switch (mode) {
            case WrapMode::Clamp:
                return MTLSamplerAddressModeClampToZero;
            case WrapMode::ClampBorder:
                return MTLSamplerAddressModeClampToBorderColor;
            case WrapMode::ClampEdge:
                return MTLSamplerAddressModeClampToEdge;
            case WrapMode::Repeat:
                return MTLSamplerAddressModeRepeat;
            case WrapMode::MirroredRepeat:
                return MTLSamplerAddressModeMirrorRepeat;
            case WrapMode::MirroredClampEdge:
                return MTLSamplerAddressModeMirrorClampToEdge;
        }
    }

    MTLPixelFormat MetalTypes::pixelFormatToMetal(PixelFormat format) {
        switch (format) {
            case PixelFormat::Red8:
                return MTLPixelFormatR8Unorm;
            case PixelFormat::Red16:
                return MTLPixelFormatR16Unorm;
            case PixelFormat::RedHalf:
                return MTLPixelFormatR16Float;
            case PixelFormat::RedFloat:
                return MTLPixelFormatR32Float;
            case PixelFormat::RG16:
                return MTLPixelFormatRG8Unorm;
            case PixelFormat::RG32:
                return MTLPixelFormatRG16Unorm;
            case PixelFormat::RGHalf:
                return MTLPixelFormatRG16Float;
            case PixelFormat::RGFloat:
                return MTLPixelFormatRG32Float;
            case PixelFormat::RGB24:  // fallback alpha with 0xFF
            case PixelFormat::RGBA32:
                return MTLPixelFormatRGBA8Unorm;
            case PixelFormat::BGRA32:
                return MTLPixelFormatBGRA8Unorm;
            case PixelFormat::RGBA64:
                return MTLPixelFormatRGBA16Unorm;
            case PixelFormat::RGBAHalf:
                return MTLPixelFormatRGBA16Float;
            case PixelFormat::RGBAFloat:
                return MTLPixelFormatRGBA32Float;
            case PixelFormat::Alpha8:
                return MTLPixelFormatA8Unorm;
            case PixelFormat::sRGB24:  // fallback alpha with 0xFF
            case PixelFormat::sRGBA32:
                return MTLPixelFormatRGBA8Unorm_sRGB;
            case PixelFormat::Depth32:
                return MTLPixelFormatDepth32Float;
            case PixelFormat::Stencil8:
                return MTLPixelFormatStencil8;
            case PixelFormat::Depth24Stencil8:
                return MTLPixelFormatX24_Stencil8;  // MTLPixelFormatDepth24Unorm_Stencil8 can't directly read the stencil value of a texture
            case PixelFormat::Depth32Stencil8:
                return MTLPixelFormatDepth32Float_Stencil8;
            default:
                LOG_ERROR(logger, "Invalid Texture format.");
                break;
        }

        return MTLPixelFormatInvalid;
    }

    MTLTextureUsage MetalTypes::textureUsageToMetal(TextureUsage usage) {
        switch (usage) {
            case TextureUsage::ShaderRead:
                return MTLTextureUsageShaderRead;
            case TextureUsage::ShaderWrite:
                return MTLTextureUsageShaderWrite;
            case TextureUsage::RenderTarget:
                return MTLTextureUsageRenderTarget;
            default:
                break;
        }

        return MTLTextureUsageUnknown;
    }
}

#endif