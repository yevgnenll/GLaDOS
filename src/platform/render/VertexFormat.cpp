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

  //////////////////////////////////////////////////////////////
  //// VertexFormatBuilder definition
  //////////////////////////////////////////////////////////////

  VertexFormatBuilder VertexFormatBuilder::withPosition() {
    mUsePosition = true;
    return *this;
  }

  VertexFormatBuilder VertexFormatBuilder::withNormal() {
    mUseNormal = true;
    return *this;
  }

  VertexFormatBuilder VertexFormatBuilder::withDiffuseTexCoord() {
    mUseDiffuseTexCoord = true;
    return *this;
  }

  VertexFormatBuilder VertexFormatBuilder::withReflectionTexCoord() {
    mUseReflectionTexCoord = true;
    return *this;
  }

  VertexFormatBuilder VertexFormatBuilder::withNormalTexCoord() {
    mUseNormalTexCoord = true;
    return *this;
  }

  VertexFormatBuilder VertexFormatBuilder::withColor() {
    mUseColor = true;
    return *this;
  }

  Vector<VertexFormat*> VertexFormatBuilder::build() const {
    Vector<VertexFormat*> vertexFormats;

    if (mUsePosition) {
      mPositionOffset = 0;  // position offset always zero
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Position, VertexAttributeType::Float4)));
    }

    if (mUseNormal) {
      mNormalOffset = VertexFormatBuilder::sumOfPreviousOffset(vertexFormats);
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Normal, VertexAttributeType::Float3)));
    }

    if (mUseDiffuseTexCoord) {
      mDiffuseTexCoordOffset = VertexFormatBuilder::sumOfPreviousOffset(vertexFormats);
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord0, VertexAttributeType::Float2)));
    }

    if (mUseReflectionTexCoord) {
      mReflectionTexCoordOffset = VertexFormatBuilder::sumOfPreviousOffset(vertexFormats);
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord1, VertexAttributeType::Float2)));
    }

    if (mUseNormalTexCoord) {
      mNormalTexCoordOffset = VertexFormatBuilder::sumOfPreviousOffset(vertexFormats);
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord2, VertexAttributeType::Float2)));
    }

    if (mUseColor) {
      mColorOffset = VertexFormatBuilder::sumOfPreviousOffset(vertexFormats);
      vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Color, VertexAttributeType::Float4)));
    }

    return vertexFormats;
  }

  std::size_t VertexFormatBuilder::sumOfPreviousOffset(const Vector<VertexFormat*>& vertexFormats) {
    std::size_t sum = 0;
    for (const auto& vertexFormat : vertexFormats) {
      sum += vertexFormat->size();
    }
    return sum;
  }
}  // namespace GLaDOS