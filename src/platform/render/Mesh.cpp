#include "Mesh.h"

namespace GLaDOS {
  PrimitiveType Mesh::getPrimitiveType() const {
    return mPrimitiveType;
  }

  void Mesh::setPrimitiveType(PrimitiveType _primitiveType) {
    mPrimitiveType = _primitiveType;
  }

  Buffer* Mesh::getVertexBuffer() const {
    return mVertexBuffer;
  }

  Buffer* Mesh::getIndexBuffer() const {
    return mIndexBuffer;
  }

  Vector<VertexFormat> Mesh::getVertexFormats() const {
    return Vector<VertexFormat>();  // TODO
  }

  uint32_t Mesh::getVertexStart() const {
    return mVertexStart;
  }

  uint32_t Mesh::getVertexCount() const {
    return mVertexCount;
  }

  uint32_t Mesh::getIndexStart() const {
    return mIndexStart;
  }

  uint32_t Mesh::getIndexCount() const {
    return mIndexCount;
  }

  uint32_t Mesh::getIndexStride() const {
    return mIndexStride;
  }
}  // namespace GLaDOS