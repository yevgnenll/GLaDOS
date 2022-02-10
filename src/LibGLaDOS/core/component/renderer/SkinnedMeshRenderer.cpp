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

    void SkinnedMeshRenderer::buildMatrixPalette(GameObject* node, Mesh* mesh, const Mat4<real>& parentMatrix, std::size_t& matrixIndex) {
        // Pre Order Traversal in children nodes
        if (node == nullptr) {
            return;
        }
        // caching to-parent matrix
        Mat4<real> transformMatrix = node->transform()->localMatrix() * parentMatrix;
        // bine pose * to root transform
        mMatrixPalette[matrixIndex++] = mesh->getBindPose(matrixIndex) * transformMatrix;

        Vector<GameObject*> children = node->getChildren();
        for (uint32_t i = 0; i < children.size(); i++) {
            buildMatrixPalette(children[i], mesh, transformMatrix, matrixIndex);
        }
    }

    void SkinnedMeshRenderer::update(real deltaTime) {
        if (mRenderable != nullptr) {
            ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
            Mesh* mesh = mRenderable->getMesh();
            std::size_t matrixIndex = 0;
            buildMatrixPalette(mRootBone, mesh, mRootBone->transform()->parentLocalMatrix(), matrixIndex);
            shaderProgram->setUniform("boneTransform", mMatrixPalette.data(), mMatrixPalette.size());
        }

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