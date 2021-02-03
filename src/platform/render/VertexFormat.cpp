#include "VertexFormat.h"

#include "utils/Utility.h"

namespace GLaDOS {
  VertexFormat::VertexFormat(VertexSemantic _semantic, VertexAttributeType _attributeType) : mSemantic{_semantic}, mType{_attributeType} {
  }

  VertexSemantic VertexFormat::semantic() const {
    return mSemantic;
  }

  VertexAttributeType VertexFormat::type() const {
    return mType;
  }

  std::size_t VertexFormat::size() const {
    switch (mType) {
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
      case VertexAttributeType::Byte:
        return 1 * sizeof(char);
      case VertexAttributeType::Byte2:
        return 2 * sizeof(char);
      case VertexAttributeType::Byte3:
        return 3 * sizeof(char);
      case VertexAttributeType::Byte4:
        return 4 * sizeof(char);
      case VertexAttributeType::UByte:
        return 1 * sizeof(char);
      case VertexAttributeType::UByte2:
        return 2 * sizeof(char);
      case VertexAttributeType::UByte3:
        return 3 * sizeof(char);
      case VertexAttributeType::UByte4:
        return 4 * sizeof(char);
      case VertexAttributeType::UByteNorm:
        return 1 * sizeof(char);
      case VertexAttributeType::UByte4Norm:
        return 4 * sizeof(char);
      case VertexAttributeType::Short2:
        return 2 * sizeof(int16_t);
      case VertexAttributeType::Short4:
        return 4 * sizeof(int16_t);
      case VertexAttributeType::UShort2:  // fall through
      case VertexAttributeType::UShort2Norm:
        return 2 * sizeof(uint16_t);
      case VertexAttributeType::Uint:
        return sizeof(uint32_t);
      case VertexAttributeType::UShortNorm:
        return sizeof(uint16_t);
      case VertexAttributeType::UShort4Norm:
        return 4 * sizeof(uint16_t);
      default:
        LOG_ERROR("Unknown vertex attribute type");
        break;
    }

    return 0;
  }

  std::size_t VertexFormat::sizeAlign4() const {
    return align4(size());
  }
}  // namespace GLaDOS