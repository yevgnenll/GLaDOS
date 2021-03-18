#ifndef GLADOS_VERTEXFORMAT_H
#define GLADOS_VERTEXFORMAT_H

#include "utils/Enumeration.h"
#include "utils/Utility.h"

namespace GLaDOS {
  class VertexFormat {
  public:
    explicit VertexFormat(VertexSemantic _semantic, VertexAttributeType _attributeType);
    VertexSemantic semantic() const;
    VertexAttributeType type() const;

    std::size_t size() const;
    std::size_t sizeAlign4() const;

  private:
    VertexSemantic mSemantic{VertexSemantic::Unknown};
    VertexAttributeType mType{VertexAttributeType::Unknown};
  };

  class VertexFormatBuilder {
    friend class VertexData;

  public:
    VertexFormatBuilder() = default;
    ~VertexFormatBuilder() = default;

    VertexFormatBuilder withPosition();
    VertexFormatBuilder withNormal();
    VertexFormatBuilder withTexCoord0();
    VertexFormatBuilder withTexCoord1();
    VertexFormatBuilder withTexCoord2();
    VertexFormatBuilder withTexCoord3();
    VertexFormatBuilder withTexCoord4();
    VertexFormatBuilder withTexCoord5();
    VertexFormatBuilder withTexCoord6();
    VertexFormatBuilder withTexCoord7();
    VertexFormatBuilder withColor();
    Vector<VertexFormat*> build() const;

  private:
    static std::size_t sumOfPreviousOffset(const Vector<VertexFormat*>& vertexFormats);

    bool mUsePosition{false};
    mutable std::size_t mPositionOffset{0};
    bool mUseNormal{false};
    mutable std::size_t mNormalOffset{0};
    bool mUseTexCoord0{false};
    mutable std::size_t mTexCoord0Offset{0};
    bool mUseTexCoord1{false};
    mutable std::size_t mTexCoord1Offset{0};
    bool mUseTexCoord2{false};
    mutable std::size_t mTexCoord2Offset{0};
    bool mUseTexCoord3{false};
    mutable std::size_t mTexCoord3Offset{0};
    bool mUseTexCoord4{false};
    mutable std::size_t mTexCoord4Offset{0};
    bool mUseTexCoord5{false};
    mutable std::size_t mTexCoord5Offset{0};
    bool mUseTexCoord6{false};
    mutable std::size_t mTexCoord6Offset{0};
    bool mUseTexCoord7{false};
    mutable std::size_t mTexCoord7Offset{0};
    bool mUseColor{false};
    mutable std::size_t mColorOffset{0};
  };
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXFORMAT_H
