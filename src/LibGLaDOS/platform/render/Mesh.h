#ifndef GLADOS_MESH_H
#define GLADOS_MESH_H

#include "GPUBuffer.h"
#include "resource/Resource.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class Logger;
    class VertexFormat;
    class VertexData;
    class IndexData;
    class VertexFormatHolder;
    class Mesh : public Resource {
      public:
        Mesh();
        Mesh(PrimitiveTopology primitiveType, GPUBufferUsage vertexBufferUsage, GPUBufferUsage indexBufferUsage);
        ~Mesh() override;

        PrimitiveTopology getPrimitiveType() const;
        void setPrimitiveType(PrimitiveTopology primitiveType);
        GPUBuffer* getVertexBuffer() const;
        GPUBuffer* getIndexBuffer() const;
        VertexFormatHolder* getVertexFormatHolder() const;
        std::size_t getVertexStart() const;
        void setVertexStart(std::size_t vertexStart);
        std::size_t getIndexStart() const;
        void setIndexStart(std::size_t indexStart);
        std::size_t getVertexCount() const;
        std::size_t getVertexStride() const;
        std::size_t getIndexCount() const;
        std::size_t getIndexStride() const;
        std::size_t getMemoryUsageInBytes() const;
        std::size_t getFaceCount() const;
        VertexData* getVertexData();
        IndexData* getIndexData();
        GPUBufferUsage getVertexUsage() const;
        GPUBufferUsage getIndexUsage() const;

        bool build(VertexData* vertexData, IndexData* indexData);
        void recalculateNormals();
        void recalculateTangent();
        void recalculateBounds();

      private:
        static Logger* logger;

        PrimitiveTopology mPrimitiveTopology{PrimitiveTopology::Triangle};
        GPUBuffer* mVertexBufferGPU{nullptr};
        GPUBuffer* mIndexBufferGPU{nullptr};
        VertexData* mVertexBufferCPU{nullptr};
        IndexData* mIndexBufferCPU{nullptr};
        std::size_t mVertexStartLocation{0};
        std::size_t mIndexStartLocation{0};
        GPUBufferUsage mVertexBufferUsage{GPUBufferUsage::Private};
        GPUBufferUsage mIndexBufferUsage{GPUBufferUsage::Private};
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESH_H
