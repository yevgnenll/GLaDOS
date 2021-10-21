#include "CommonTypes.h"
#include "utils/Utility.h"

namespace GLaDOS {
    Logger* CommonTypes::logger = LoggerRegistry::getInstance().makeAndGetLogger("CommonTypes");

    std::size_t CommonTypes::vertexArrtibTypeToSize(VertexAttributeType type) {
        switch (type) {
            case VertexAttributeType::Float:
                return sizeof(float);
            case VertexAttributeType::Float2:
                return 2 * sizeof(float);
            case VertexAttributeType::Float3:
                return 3 * sizeof(float);
            case VertexAttributeType::Float4:
                return 4 * sizeof(float);

            case VertexAttributeType::Half:
                return sizeof(uint16_t);
            case VertexAttributeType::Half2:
                return 2 * sizeof(uint16_t);
            case VertexAttributeType::Half3:
                return 3 * sizeof(uint16_t);
            case VertexAttributeType::Half4:
                return 4 * sizeof(uint16_t);

            case VertexAttributeType::Int:
                return sizeof(int32_t);
            case VertexAttributeType::Int2:
                return 2 * sizeof(int32_t);
            case VertexAttributeType::Int3:
                return 3 * sizeof(int32_t);
            case VertexAttributeType::Int4:
                return 4 * sizeof(int32_t);

            case VertexAttributeType::UInt:
                return sizeof(uint32_t);
            case VertexAttributeType::UInt2:
                return 2 * sizeof(uint32_t);
            case VertexAttributeType::UInt3:
                return 3 * sizeof(uint32_t);
            case VertexAttributeType::UInt4:
                return 4 * sizeof(uint32_t);

            case VertexAttributeType::Byte:
                return 1 * sizeof(char);
            case VertexAttributeType::Byte2:
                return 2 * sizeof(char);
            case VertexAttributeType::Byte3:
                return 3 * sizeof(char);
            case VertexAttributeType::Byte4:
                return 4 * sizeof(char);

            case VertexAttributeType::UByte:
                return 1 * sizeof(unsigned char);
            case VertexAttributeType::UByte2:
                return 2 * sizeof(unsigned char);
            case VertexAttributeType::UByte3:
                return 3 * sizeof(unsigned char);
            case VertexAttributeType::UByte4:
                return 4 * sizeof(unsigned char);

            case VertexAttributeType::Short:
                return 1 * sizeof(int16_t);
            case VertexAttributeType::Short2:
                return 2 * sizeof(int16_t);
            case VertexAttributeType::Short3:
                return 3 * sizeof(int16_t);
            case VertexAttributeType::Short4:
                return 4 * sizeof(int16_t);

            case VertexAttributeType::UShort:
                return 1 * sizeof(uint16_t);
            case VertexAttributeType::UShort2:
                return 2 * sizeof(uint16_t);
            case VertexAttributeType::UShort3:
                return 3 * sizeof(uint16_t);
            case VertexAttributeType::UShort4:
                return 4 * sizeof(uint16_t);

            default:
                LOG_ERROR(logger, "Unknown vertex attribute type");
                break;
        }

        return 0;
    }

    std::size_t CommonTypes::uniformTypeToSize(UniformType uniformType) {
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
            case UniformType::Mat2:
                return 16;
            case UniformType::Mat3:
                return 36;
            case UniformType::Mat4:
                return 64;
            case UniformType::Texture:
                return 4;
            case UniformType::Sampler:
                return 4;
            default:
                LOG_ERROR(logger, "Unknown uniform type");
                break;
        }

        return 0;
    }

    const char* CommonTypes::vertexSemanticToName(VertexSemantic semantic) {
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
                LOG_WARN(logger, "Not supported vertex semantic type");
                break;
        }

        return "_unknown";
    }
}