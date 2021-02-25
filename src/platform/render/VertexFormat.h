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
    VertexFormatBuilder withDiffuseTexCoord();
    VertexFormatBuilder withReflectionTexCoord();
    VertexFormatBuilder withNormalTexCoord();
    VertexFormatBuilder withColor();
    Vector<VertexFormat*> build() const;

  private:
    static std::size_t sumOfPreviousOffset(const Vector<VertexFormat*>& vertexFormats);

    bool mUsePosition{false};
    mutable std::size_t mPositionOffset{0};
    bool mUseNormal{false};
    mutable std::size_t mNormalOffset{0};
    bool mUseDiffuseTexCoord{false};
    mutable std::size_t mDiffuseTexCoordOffset{0};
    bool mUseReflectionTexCoord{false};
    mutable std::size_t mReflectionTexCoordOffset{0};
    bool mUseNormalTexCoord{false};
    mutable std::size_t mNormalTexCoordOffset{0};
    bool mUseColor{false};
    mutable std::size_t mColorOffset{0};
  };
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXFORMAT_H
