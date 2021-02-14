#ifndef GLADOS_MESH_H
#define GLADOS_MESH_H

#include "Buffer.h"
#include "VertexFormat.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class Mesh {
  public:
    Mesh() = default;
    ~Mesh() = default;

    PrimitiveType getPrimitiveType() const;
    void setPrimitiveType(PrimitiveType _primitiveType);
    Buffer* getVertexBuffer() const;
    Buffer* getIndexBuffer() const;
    Vector<VertexFormat> getVertexFormats() const;
    uint32_t getVertexStart() const;
    uint32_t getVertexCount() const;
    uint32_t getIndexStart() const;
    uint32_t getIndexCount() const;
    uint32_t getIndexStride() const;

  private:
    PrimitiveType mPrimitiveType{PrimitiveType::Triangle};
    Buffer* mVertexBuffer{nullptr};
    Buffer* mIndexBuffer{nullptr};
    uint32_t mVertexStart;
    uint32_t mVertexCount;
    uint32_t mIndexStart;
    uint32_t mIndexCount;
    uint32_t mIndexStride;
  };
}  // namespace GLaDOS

#endif  //GLADOS_MESH_H
