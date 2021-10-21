#include "MetalTypes.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
    Logger* MetalTypes::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalTypes");

    constexpr MTLVertexFormat MetalTypes::vertexAttribTypeToVertexFormat(VertexAttributeType type, bool normalized) {
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

    constexpr std::size_t MetalTypes::metalVertexFormatToSize(MTLVertexFormat vertexFormat) {
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

    constexpr UniformType MetalTypes::metalDataTypeToUniformType(MTLDataType dataType) {
        switch (dataType) {
            case MTLDataTypeBool:      return UniformType::Bool;
            case MTLDataTypeInt:       return UniformType::Int;
            case MTLDataTypeUInt:      return UniformType::UInt;
            case MTLDataTypeFloat:     return UniformType::Float;
            case MTLDataTypeFloat2:    return UniformType::Vec2;
            case MTLDataTypeFloat3:    return UniformType::Vec3;
            case MTLDataTypeFloat4:    return UniformType::Vec4;
            case MTLDataTypeFloat4x4:  return UniformType::Mat4;
            case MTLDataTypeTexture:   return UniformType::Texture;
            case MTLDataTypeSampler:   return UniformType::Sampler;
            default:
                LOG_ERROR(logger, "Unknown data type");
                break;
        }

        return UniformType::Unknown;
    }

    constexpr MTLTriangleFillMode MetalTypes::fillModeToMetalFillMode(FillMode mode) {
        switch (mode) {
            case FillMode::Fill:   return MTLTriangleFillModeFill;
            case FillMode::Lines:  return MTLTriangleFillModeLines;
        }
    }

    constexpr MTLWinding MetalTypes::windingModeToMetalWinding(WindingMode mode) {
        switch (mode) {
            case WindingMode::ClockWise:         return MTLWindingClockwise;
            case WindingMode::CounterClockWise:  return MTLWindingCounterClockwise;
        }
    }

    constexpr MTLCullMode MetalTypes::cullModeToMetalCullMode(CullMode mode) {
        switch (mode) {
            case CullMode::None:   return MTLCullModeNone;
            case CullMode::Front:  return MTLCullModeFront;
            case CullMode::Back:   return MTLCullModeBack;
        }
    }

    constexpr MTLVertexFormat MetalTypes::metalDataTypeToVertexFormat(MTLDataType dataType, bool normalized) {
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
}

#endif