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
    DELETE_T(mVertexBuffer, GPUBuffer);
    DELETE_T(mIndexBuffer, GPUBuffer);
    DELETE_T(mVertexData, VertexData);
    DELETE_T(mIndexData, IndexData);
  }

  PrimitiveType Mesh::getPrimitiveType() const {
    return mPrimitiveType;
  }

  void Mesh::setPrimitiveType(PrimitiveType primitiveType) {
    mPrimitiveType = primitiveType;
  }

  GPUBuffer* Mesh::getVertexBuffer() const {
    return mVertexBuffer;
  }

  GPUBuffer* Mesh::getIndexBuffer() const {
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
    return mIndexData == nullptr ? 0 : mIndexData->count();
  }

  std::size_t Mesh::getIndexStride() const {
    return mIndexData == nullptr ? 0 : mIndexData->stride();
  }

  std::size_t Mesh::getMemoryUsage() const {
    std::size_t vertexSize = mVertexData->size();
    return mIndexData == nullptr ? vertexSize : vertexSize + mIndexData->size();
  }

  std::size_t Mesh::getFaceCount() const {
    std::size_t count = mIndexData == nullptr ? mVertexData->count() : mIndexData->count();
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

  VertexData* Mesh::getVertexData() {
    return mVertexData;
  }

  IndexData* Mesh::getIndexData() {
    return mIndexData;
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
      LOG_ERROR("Vertex data must not be null.");
      return false;
    }

    BufferUsage vertexUsage = mIsDynamicVertex ? BufferUsage::Dynamic : BufferUsage::Static;
    mVertexBuffer = Platform::getRenderer()->createVertexBuffer(vertexUsage, vertexData->buffer());
    if (mVertexBuffer == nullptr) {
      LOG_ERROR("VertexBuffer creation failed.");
      return false;
    }

    if (mIndexData != nullptr) {
      BufferUsage indexUsage = mIsDynamicIndex ? BufferUsage::Dynamic : BufferUsage::Static;
      mIndexBuffer = Platform::getRenderer()->createIndexBuffer(indexUsage, mIndexData->buffer());
      if (mIndexBuffer == nullptr) {
        LOG_ERROR("IndexBuffer creation failed.");
        return false;
      }
    }

    return true;
  }

  void Mesh::recalculateNormals() {
    // TODO
  }

  void Mesh::recalculateTangent() {
    // TODO
  }

  void Mesh::recalculateBounds() {
    // TODO
  }

}  // namespace GLaDOS