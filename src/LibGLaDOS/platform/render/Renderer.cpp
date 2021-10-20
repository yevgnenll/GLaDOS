#include "Renderer.h"
#include "platform/render/Mesh.h"

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

    Mesh* Renderer::createMesh(const std::string& meshPath, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage) {
        // TODO
        // const auto& [vertexBuffer, indexBuffer] = MeshLoader::loadFromFile(meshPath);
        // return createMesh(vertexBuffer, indexBuffer, primitiveTopology, dynamicVertex, dynamicIndex);
        return nullptr;
    }
}