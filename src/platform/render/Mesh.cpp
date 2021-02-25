#include "Mesh.h"

#include "IndexData.h"
#include "Renderer.h"
#include "RootDir.h"
#include "VertexData.h"

namespace GLaDOS {
  Mesh::Mesh(PrimitiveType primitiveType, bool dynamicVertex, bool dynamicIndex)
      : Resource{ResourceType::Mesh}, mPrimitiveType{primitiveType}, mIsDynamicVertex{dynamicVertex}, mIsDynamicIndex{dynamicIndex} {
    setResourceDir(RESOURCE_DIR);
  }

  Mesh::~Mesh() {
    DELETE_T(mVertexBuffer, Buffer);
    DELETE_T(mIndexBuffer, Buffer);
    DELETE_T(mVertexData, VertexData);
    DELETE_T(mIndexData, IndexData);
  }

  PrimitiveType Mesh::getPrimitiveType() const {
    return mPrimitiveType;
  }

  void Mesh::setPrimitiveType(PrimitiveType primitiveType) {
    mPrimitiveType = primitiveType;
  }

  Buffer* Mesh::getVertexBuffer() const {
    return mVertexBuffer;
  }

  Buffer* Mesh::getIndexBuffer() const {
    return mIndexBuffer;
  }

  Vector<VertexFormat*> Mesh::getVertexFormats() const {
    return mVertexData->getVertexFormats();
  }

  std::size_t Mesh::getVertexStart() const {
    return mVertexStart;
  }

  void Mesh::setVertexStart(std::size_t vertexStart) {
    mVertexStart = vertexStart;
  }

  std::size_t Mesh::getIndexStart() const {
    return mIndexStart;
  }

  void Mesh::setIndexStart(std::size_t indexStart) {
    mIndexStart = indexStart;
  }

  std::size_t Mesh::getVertexCount() const {
    return mVertexData->count();
  }

  std::size_t Mesh::getVertexStride() const {
    return mVertexData->stride();
  }

  std::size_t Mesh::getIndexCount() const {
    return mIndexData->count();
  }

  std::size_t Mesh::getIndexStride() const {
    return mIndexData->stride();
  }

  std::size_t Mesh::getMemoryUsage() const {
    return mVertexData->size() + mIndexData->size();
  }

  std::size_t Mesh::getFaceCount() const {
    std::size_t count = mIndexData != nullptr ? mIndexData->count() : mVertexData->count();
    switch (mPrimitiveType) {
      case PrimitiveType::Point:
        return count;
      case PrimitiveType::Line:
        return count / 2;
      case PrimitiveType::LineStrip:
        return count - 1;
      case PrimitiveType::Triangle:
        return count / 3;
      case PrimitiveType::TriangleStrip:
        return count - 2;
    }
  }

  bool Mesh::isDynamicVertex() const {
    return mIsDynamicVertex;
  }

  bool Mesh::isDynamicIndex() const {
    return mIsDynamicIndex;
  }

  bool Mesh::build(VertexData* vertexData, IndexData* indexData) {
    mVertexData = vertexData;
    mIndexData = indexData;

    if (mVertexData == nullptr) {
      LOG_ERROR("Vertex data must not be null");
      return false;
    }

    BufferUsage vertexUsage = mIsDynamicVertex ? BufferUsage::Dynamic : BufferUsage::Static;
    mVertexBuffer = Platform::getRenderer()->createVertexBuffer(vertexUsage, vertexData->buffer());

    if (mVertexBuffer == nullptr) {
      return false;
    }

    if (mIndexData != nullptr) {
      BufferUsage indexUsage = mIsDynamicVertex ? BufferUsage::Dynamic : BufferUsage::Static;
      mIndexBuffer = Platform::getRenderer()->createIndexBuffer(indexUsage, indexData->buffer());
      if (mIndexBuffer == nullptr) {
        return false;
      }
    }

    return true;
  }

  void Mesh::RecalculateNormals() {
    // TODO
  }

  void Mesh::RecalculateTangent() {
    // TODO
  }

  void Mesh::RecalculateBounds() {
    // TODO
  }

}  // namespace GLaDOS