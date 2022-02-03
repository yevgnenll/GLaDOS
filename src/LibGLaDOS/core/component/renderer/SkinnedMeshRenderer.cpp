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
        mMatrixPalette.resize(MAX_BONE_MATRIX); // fixed size maximum bone transform matrix
    }

    SkinnedMeshRenderer::SkinnedMeshRenderer(Mesh *mesh, Material *material, GameObject* rootBone)
        : MeshRenderer(mesh, material), mRootBone{rootBone} {
        mMatrixPalette.resize(MAX_BONE_MATRIX); // fixed size maximum bone transform matrix
    }

    SkinnedMeshRenderer::~SkinnedMeshRenderer() {
    }

    void SkinnedMeshRenderer::setRootBone(GameObject* gameObject) {
        mRootBone = gameObject;
    }

    void SkinnedMeshRenderer::preOrderTraversal(Vector<Mat4<real>>& matrixPalette, GameObject* node, std::size_t& matrixIndex) {
        if (node == nullptr) {
            return;
        }
        matrixPalette[matrixIndex++] = node->transform()->localToWorldMatrix();
        Vector<GameObject*> children = node->getChildren();
        for (uint32_t i = 0; i < children.size(); i++) {
            preOrderTraversal(matrixPalette, children[i], matrixIndex);
        }
    }

    void SkinnedMeshRenderer::update(real deltaTime) {
        ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
        std::size_t matrixIndex = 0;
        preOrderTraversal(mMatrixPalette, mRootBone, matrixIndex);
        shaderProgram->setUniform("boneTransform", mMatrixPalette.data(), mMatrixPalette.size());
        MeshRenderer::update(deltaTime);
    }

    void SkinnedMeshRenderer::render() {
        MeshRenderer::render();
    }

    Component* SkinnedMeshRenderer::clone() {
        // TODO
        return nullptr;
    }
}