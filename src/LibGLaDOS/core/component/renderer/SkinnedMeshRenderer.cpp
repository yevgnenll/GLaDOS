#include "SkinnedMeshRenderer.h"
#include "platform/Platform.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/ShaderProgram.h"
#include "core/GameObject.hpp"
#include "core/component/Transform.h"

namespace GLaDOS {
    Logger* SkinnedMeshRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("SkinnedMeshRenderer");
    SkinnedMeshRenderer::SkinnedMeshRenderer() {
        mName = "SkinnedMeshRenderer";
    }

    SkinnedMeshRenderer::SkinnedMeshRenderer(Mesh *mesh, Material *material, GameObject* rootBone)
        : MeshRenderer(mesh, material), mRootBone{rootBone} {
    }

    SkinnedMeshRenderer::~SkinnedMeshRenderer() {
    }

    void SkinnedMeshRenderer::setRootBone(GameObject* gameObject) {
        mRootBone = gameObject;
    }

    void SkinnedMeshRenderer::buildMatrixPalette(const Mat4<real>& parentMatrix, GameObject* node, std::size_t& matrixIndex) {
        // Pre Order Traversal in children nodes
        if (node == nullptr) {
            return;
        }
        // inverse bine pose matrix * bone transformation
        mMatrixPalette[matrixIndex++] = node->transform()->worldToLocalMatrix() * parentMatrix;
        Vector<GameObject*> children = node->getChildren();
        for (uint32_t i = 0; i < children.size(); i++) {
            buildMatrixPalette(parentMatrix, children[i], matrixIndex);
        }
    }

    void SkinnedMeshRenderer::update(real deltaTime) {
        if (mRenderable != nullptr) {
            ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
            Transform* transform = gameObject()->transform();
            std::size_t matrixIndex = 0;
            buildMatrixPalette(transform->localToWorldMatrix(), mRootBone, matrixIndex);
            shaderProgram->setUniform("boneTransform", mMatrixPalette.data(), mMatrixPalette.size());
            MeshRenderer::update(deltaTime);
        }
    }

    void SkinnedMeshRenderer::render() {
        MeshRenderer::render();
    }

    Component* SkinnedMeshRenderer::clone() {
        // TODO
        return nullptr;
    }
}