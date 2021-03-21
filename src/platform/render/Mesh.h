#ifndef GLADOS_MESH_H
#define GLADOS_MESH_H

#include "GPUBuffer.h"
#include "resource/Resource.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class VertexFormat;
  class VertexData;
  class IndexData;
  class Mesh : public Resource {
  public:
    Mesh(PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex);
    ~Mesh() override;

    PrimitiveType getPrimitiveType() const;
    void setPrimitiveType(PrimitiveType primitiveType);
    GPUBuffer* getVertexBuffer() const;
    GPUBuffer* getIndexBuffer() const;
    Vector<VertexFormat*> getVertexFormats() const;
    std::size_t getVertexStart() const;
    void setVertexStart(std::size_t vertexStart);
    std::size_t getIndexStart() const;
    void setIndexStart(std::size_t indexStart);
    std::size_t getVertexCount() const;
    std::size_t getVertexStride() const;
    std::size_t getIndexCount() const;
    std::size_t getIndexStride() const;
    std::size_t getMemoryUsage() const;
    std::size_t getFaceCount() const;
    VertexData* getVertexData();
    IndexData* getIndexData();
    bool isDynamicVertex() const;
    bool isDynamicIndex() const;

    bool build(VertexData* vertexData, IndexData* indexData);
    void recalculateNormals();
    void recalculateTangent();
    void recalculateBounds();

  private:
    PrimitiveType mPrimitiveType;
    GPUBuffer* mVertexBuffer{nullptr};
    GPUBuffer* mIndexBuffer{nullptr};
    VertexData* mVertexData{nullptr};
    IndexData* mIndexData{nullptr};
    std::size_t mVertexStart{0};
    std::size_t mIndexStart{0};
    bool mIsDynamicVertex{false};  // TODO
    bool mIsDynamicIndex{false};  // TODO
  };
}  // namespace GLaDOS

#endif  //GLADOS_MESH_H
