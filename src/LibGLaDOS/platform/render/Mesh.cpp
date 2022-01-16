#include "Mesh.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "platform/Platform.h"

namespace GLaDOS {
    Logger* Mesh::logger = LoggerRegistry::getInstance().makeAndGetLogger("Mesh");
    Mesh::Mesh() {
    }

    Mesh::Mesh(PrimitiveTopology primitiveType, GPUBufferUsage vertexBufferUsage, GPUBufferUsage indexBufferUsage)
        : mPrimitiveTopology{primitiveType}, mVertexBufferUsage{vertexBufferUsage}, mIndexBufferUsage{indexBufferUsage} {
    }

    Mesh::~Mesh() {
        DELETE_T(mVertexBufferGPU, GPUBuffer);
        DELETE_T(mIndexBufferGPU, GPUBuffer);
        DELETE_T(mVertexBufferCPU, VertexBuffer);
        DELETE_T(mIndexBufferCPU, IndexBuffer);
    }

    Mesh::Mesh(const Mesh& other)
        : mPrimitiveTopology{other.mPrimitiveTopology}, mVertexStartLocation{other.mVertexStartLocation}, mIndexStartLocation{other.mIndexStartLocation},
            mVertexBufferUsage{other.mVertexBufferUsage}, mIndexBufferUsage{other.mIndexBufferUsage} {
        VertexBuffer* vertexBuffer = Platform::getRenderer().createVertexBuffer(other.mVertexBufferCPU->getVertexFormatDescriptor(), other.mVertexBufferCPU->count());
        vertexBuffer->copyBufferData(other.mVertexBufferCPU->buffer());
        IndexBuffer* indexBuffer = nullptr;
        if (other.mIndexBufferCPU != nullptr) {
            indexBuffer = Platform::getRenderer().createIndexBuffer(other.mIndexBufferCPU->stride(), other.mIndexBufferCPU->count());
            indexBuffer->copyBufferData(other.mIndexBufferCPU->buffer());
        }
        build(vertexBuffer, indexBuffer);
    }

    Mesh& Mesh::operator=(const Mesh& other) {
        mPrimitiveTopology = other.mPrimitiveTopology;
        mVertexStartLocation = other.mVertexStartLocation;
        mIndexStartLocation = other.mIndexStartLocation;
        mVertexBufferUsage = other.mVertexBufferUsage;
        mIndexBufferUsage = other.mIndexBufferUsage;
        VertexBuffer* vertexBuffer = Platform::getRenderer().createVertexBuffer(other.mVertexBufferCPU->getVertexFormatDescriptor(), other.mVertexBufferCPU->count());
        vertexBuffer->copyBufferData(other.mVertexBufferCPU->buffer());
        IndexBuffer* indexBuffer = nullptr;
        if (other.mIndexBufferCPU != nullptr) {
            indexBuffer = Platform::getRenderer().createIndexBuffer(other.mIndexBufferCPU->stride(), other.mIndexBufferCPU->count());
            indexBuffer->copyBufferData(other.mIndexBufferCPU->buffer());
        }
        build(vertexBuffer, indexBuffer);
        return *this;
    }

    PrimitiveTopology Mesh::getPrimitiveType() const {
        return mPrimitiveTopology;
    }

    void Mesh::setPrimitiveType(PrimitiveTopology primitiveType) {
        mPrimitiveTopology = primitiveType;
    }

    GPUBuffer* Mesh::getGPUVertexBuffer() const {
        return mVertexBufferGPU;
    }

    GPUBuffer* Mesh::getGPUIndexBuffer() const {
        return mIndexBufferGPU;
    }

    VertexFormatHolder* Mesh::getVertexFormatHolder() const {
        return mVertexBufferCPU->getVertexFormatHolder();
    }

    std::size_t Mesh::getVertexStart() const {
        return mVertexStartLocation;
    }

    void Mesh::setVertexStart(std::size_t vertexStart) {
        if (vertexStart < 0 || vertexStart > mVertexBufferCPU->size()) {
            LOG_ERROR(logger, "Mesh vertex start position must be between 0 and {0},", mVertexBufferCPU->size());
        }
        mVertexStartLocation = vertexStart;
    }

    std::size_t Mesh::getIndexStart() const {
        return mIndexStartLocation;
    }

    void Mesh::setIndexStart(std::size_t indexStart) {
        if (indexStart < 0 || indexStart > mIndexBufferCPU->size()) {
            LOG_ERROR(logger, "Mesh index start position must be between 0 and {0},", mIndexBufferCPU->size());
        }
        mIndexStartLocation = indexStart;
    }

    std::size_t Mesh::getVertexCount() const {
        return mVertexBufferCPU->count();
    }

    std::size_t Mesh::getVertexStride() const {
        return mVertexBufferCPU->stride();
    }

    std::size_t Mesh::getIndexCount() const {
        return mIndexBufferCPU == nullptr ? 0 : mIndexBufferCPU->count();
    }

    std::size_t Mesh::getIndexStride() const {
        return mIndexBufferCPU == nullptr ? 0 : mIndexBufferCPU->stride();
    }

    std::size_t Mesh::getMemoryUsageInBytes() const {
        std::size_t vertexSize = mVertexBufferCPU->size();
        return mIndexBufferCPU == nullptr ? vertexSize : vertexSize + mIndexBufferCPU->size();
    }

    std::size_t Mesh::getFaceCount() const {
        std::size_t count = mIndexBufferCPU == nullptr ? (mVertexBufferCPU->count() - mVertexStartLocation) : (mIndexBufferCPU->count() - mIndexStartLocation);
        switch (mPrimitiveTopology) {
            case PrimitiveTopology::Point:
                return count;
            case PrimitiveTopology::Line:
                return count / 2;
            case PrimitiveTopology::LineStrip:
                return count - 1;
            case PrimitiveTopology::Triangle:
                return count / 3;
            case PrimitiveTopology::TriangleStrip:
                return count - 2;
        }
        return 0;
    }

    VertexBuffer* Mesh::getCPUVertexBuffer() {
        return mVertexBufferCPU;
    }

    IndexBuffer* Mesh::getCPUIndexBuffer() {
        return mIndexBufferCPU;
    }

    GPUBufferUsage Mesh::getVertexUsage() const {
        return mVertexBufferUsage;
    }

    GPUBufferUsage Mesh::getIndexUsage() const {
        return mIndexBufferUsage;
    }

    bool Mesh::build(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
        mVertexBufferCPU = vertexBuffer;
        mIndexBufferCPU = indexBuffer;

        if (mVertexBufferCPU == nullptr) {
            LOG_ERROR(logger, "Mesh should have at least a vertexBuffer.");
            return false;
        }

        mVertexBufferGPU = Platform::getRenderer().createGPUVertexBuffer(mVertexBufferUsage, mVertexBufferCPU->buffer(), mVertexBufferCPU->size());
        if (mVertexBufferGPU == nullptr) {
            LOG_ERROR(logger, "VertexBuffer creation failed.");
            return false;
        }

        if (mIndexBufferCPU != nullptr) {
            mIndexBufferGPU = Platform::getRenderer().createGPUIndexBuffer(mIndexBufferUsage, mIndexBufferCPU->buffer(), mIndexBufferCPU->size());
            if (mIndexBufferGPU == nullptr) {
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