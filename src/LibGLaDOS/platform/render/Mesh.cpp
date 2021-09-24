#include "Mesh.h"

#include "IndexData.h"
#include "Renderer.h"
#include "RootDir.h"
#include "VertexData.h"
#include "platform/Platform.h"

namespace GLaDOS {
    Logger* Mesh::logger = LoggerRegistry::getInstance().makeAndGetLogger("Mesh");
    Mesh::Mesh() : Resource{ResourceType::Mesh} {
        setResourceDir(RESOURCE_DIR);
    }

    Mesh::Mesh(PrimitiveType primitiveType, BufferUsage vertexBufferUsage, BufferUsage indexBufferUsage)
        : Resource{ResourceType::Mesh}, mPrimitiveType{primitiveType}, mVertexBufferUsage{vertexBufferUsage}, mIndexBufferUsage{indexBufferUsage} {
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

    VertexFormatHolder* Mesh::getVertexFormatHolder() const {
        return mVertexData->getVertexFormatHolder();
    }

    std::size_t Mesh::getVertexStart() const {
        return mVertexStart;
    }

    void Mesh::setVertexStart(std::size_t vertexStart) {
        if (vertexStart < 0 || vertexStart > mVertexData->size()) {
            LOG_ERROR(logger, "Mesh vertex start position must be between 0 and {0},", mVertexData->size());
        }
        mVertexStart = vertexStart;
    }

    std::size_t Mesh::getIndexStart() const {
        return mIndexStart;
    }

    void Mesh::setIndexStart(std::size_t indexStart) {
        if (indexStart < 0 || indexStart > mIndexData->size()) {
            LOG_ERROR(logger, "Mesh index start position must be between 0 and {0},", mIndexData->size());
        }
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

    std::size_t Mesh::getMemoryUsageInBytes() const {
        std::size_t vertexSize = mVertexData->size();
        return mIndexData == nullptr ? vertexSize : vertexSize + mIndexData->size();
    }

    std::size_t Mesh::getFaceCount() const {
        std::size_t count = mIndexData == nullptr ? (mVertexData->count() - mVertexStart) : (mIndexData->count() - mIndexStart);
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

    BufferUsage Mesh::getVertexUsage() const {
        return mVertexBufferUsage;
    }

    BufferUsage Mesh::getIndexUsage() const {
        return mIndexBufferUsage;
    }

    bool Mesh::build(VertexData* vertexData, IndexData* indexData) {
        mVertexData = vertexData;
        mIndexData = indexData;

        if (mVertexData == nullptr) {
            LOG_ERROR(logger, "Mesh should have at least a vertexData.");
            return false;
        }

        mVertexBuffer = Platform::getRenderer().createVertexBuffer(mVertexBufferUsage, mVertexData->buffer(), mVertexData->size());
        if (mVertexBuffer == nullptr) {
            LOG_ERROR(logger, "VertexBuffer creation failed.");
            return false;
        }

        if (mIndexData != nullptr) {
            mIndexBuffer = Platform::getRenderer().createIndexBuffer(mIndexBufferUsage, mIndexData->buffer(), mIndexData->size());
            if (mIndexBuffer == nullptr) {
                LOG_ERROR(logger, "IndexBuffer creation failed.");
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