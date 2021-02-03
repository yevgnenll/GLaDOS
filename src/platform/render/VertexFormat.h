#ifndef GLADOS_VERTEXFORMAT_H
#define GLADOS_VERTEXFORMAT_H

#include "utils/Enumeration.h"

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
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXFORMAT_H
