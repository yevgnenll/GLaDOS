#ifndef GLADOS_MESH_H
#define GLADOS_MESH_H

#include "GPUBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    template <typename T>
    class Mat4;
    class Logger;
    class VertexFormat;
    class VertexBuffer;
    class IndexBuffer;
    class VertexFormatHolder;
    class Mesh {
      public:
        Mesh();
        Mesh(PrimitiveTopology primitiveType, GPUBufferUsage vertexBufferUsage, GPUBufferUsage indexBufferUsage);
        ~Mesh();

        Mesh(const Mesh& other);
        Mesh& operator=(const Mesh& other);

        PrimitiveTopology getPrimitiveType() const;
        void setPrimitiveType(PrimitiveTopology primitiveType);
        GPUBuffer* getGPUVertexBuffer() const;
        GPUBuffer* getGPUIndexBuffer() const;
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
        VertexBuffer* getCPUVertexBuffer();
        IndexBuffer* getCPUIndexBuffer();
        GPUBufferUsage getVertexUsage() const;
        GPUBufferUsage getIndexUsage() const;
        Mat4<real> getBindPose(std::size_t index);
        void setBindPose(const Vector<Mat4<real>>& bindPose);

        bool build(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
        void recalculateNormals();
        void recalculateTangent();
        void recalculateBounds();

      private:
        static Logger* logger;

        PrimitiveTopology mPrimitiveTopology{PrimitiveTopology::Triangle};
        GPUBuffer* mVertexBufferGPU{nullptr};
        GPUBuffer* mIndexBufferGPU{nullptr};
        VertexBuffer* mVertexBufferCPU{nullptr};
        IndexBuffer* mIndexBufferCPU{nullptr};
        std::size_t mVertexStartLocation{0};
        std::size_t mIndexStartLocation{0};
        GPUBufferUsage mVertexBufferUsage{GPUBufferUsage::Private};
        GPUBufferUsage mIndexBufferUsage{GPUBufferUsage::Private};
        Vector<Mat4<real>> mBindPose; // the inverse of the transformation matrix of the bone relative to parent.
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESH_H
