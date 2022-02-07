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

    void SkinnedMeshRenderer::buildMatrixPalette(GameObject* node, Mesh* mesh, std::size_t& matrixIndex) {
        // Pre Order Traversal in children nodes
        if (node == nullptr) {
            return;
        }
        // bine pose * to root transform
        // TODO: localToWorldMatrix 에 최상위 부모의 transform 여러번 곱해짐. 테스트할땐 단위 행렬이라 문제없지만, 수정해야함
        mMatrixPalette[matrixIndex] = mesh->getBindPose(matrixIndex) * node->transform()->localToWorldMatrix();
        matrixIndex++;
        Vector<GameObject*> children = node->getChildren();
        for (uint32_t i = 0; i < children.size(); i++) {
            buildMatrixPalette(children[i], mesh, matrixIndex);
        }
    }

    void SkinnedMeshRenderer::update(real deltaTime) {
        if (mRenderable != nullptr) {
            ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
            Mesh* mesh = mRenderable->getMesh();
            std::size_t matrixIndex = 0;
            buildMatrixPalette(mRootBone, mesh, matrixIndex);
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