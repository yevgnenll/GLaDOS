#include "Renderer.h"
#include "platform/render/Mesh.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/IndexBuffer.h"
#include "core/GameObject.hpp"
#include "core/loader/AssimpLoader.h"

namespace GLaDOS {
    Logger* Renderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("Renderer");

    Mesh* Renderer::createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage) {
        Mesh* mesh = NEW_T(Mesh(primitiveTopology, vertexUsage, indexUsage));
        if (!mesh->build(vertexBuffer, indexBuffer)) {
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        return mesh;
    }

    Mesh* Renderer::createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
        Mesh* mesh = NEW_T(Mesh);
        if (!mesh->build(vertexBuffer, indexBuffer)) {
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        return mesh;
    }

    bool Renderer::createPrefabFromFile(const std::string& meshPath, GameObject* parent) {
        return AssimpLoader::getInstance().loadFromFile(meshPath, parent);
    }

    VertexBuffer* Renderer::createVertexBuffer(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count) {
        return NEW_T(VertexBuffer(vertexFormatDescriptor, count));
    }

    IndexBuffer* Renderer::createIndexBuffer(std::size_t stride, std::size_t count) {
        return NEW_T(IndexBuffer(stride, count));
    }
}