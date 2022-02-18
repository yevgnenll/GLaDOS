#include "Renderer.h"
#include "platform/render/Mesh.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/IndexBuffer.h"
#include "core/GameObject.hpp"
#include "core/loader/AssimpLoader.h"
#include "resource/ResourceManager.h"

namespace GLaDOS {
    Logger* Renderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("Renderer");

    Mesh* Renderer::createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology) {
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Mesh);
        if (resource != nullptr) {
            return static_cast<Mesh*>(resource);
        }

        Mesh* mesh = NEW_T(Mesh(name, primitiveTopology, GPUBufferUsage::Private, GPUBufferUsage::Private));
        if (!mesh->build(vertexBuffer, indexBuffer)) {
            DELETE_T(mesh, Mesh);
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        ResourceManager::getInstance().store(mesh);

        return mesh;
    }

    Mesh* Renderer::createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage) {
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Mesh);
        if (resource != nullptr) {
            return static_cast<Mesh*>(resource);
        }

        Mesh* mesh = NEW_T(Mesh(name, primitiveTopology, vertexUsage, indexUsage));
        if (!mesh->build(vertexBuffer, indexBuffer)) {
            DELETE_T(mesh, Mesh);
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        ResourceManager::getInstance().store(mesh);

        return mesh;
    }

    Mesh* Renderer::createMesh(const std::string& name, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
        Resource* resource = ResourceManager::getInstance().getResource(name, ResourceType::Mesh);
        if (resource != nullptr) {
            return static_cast<Mesh*>(resource);
        }

        Mesh* mesh = NEW_T(Mesh(name));
        if (!mesh->build(vertexBuffer, indexBuffer)) {
            DELETE_T(mesh, Mesh);
            LOG_ERROR(logger, "Failed to build mesh");
            return nullptr;
        }
        ResourceManager::getInstance().store(mesh);

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