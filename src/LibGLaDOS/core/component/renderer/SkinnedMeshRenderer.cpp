#include "SkinnedMeshRenderer.h"
#include "platform/Platform.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/ShaderProgram.h"

namespace GLaDOS {
    Logger* SkinnedMeshRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("SkinnedMeshRenderer");
    SkinnedMeshRenderer::SkinnedMeshRenderer() {
        mName = "SkinnedMeshRenderer";
        mMatrixPalette.resize(MAX_BONE_MATRIX);
    }

    SkinnedMeshRenderer::SkinnedMeshRenderer(Mesh *mesh, Material *material, GameObject* rootBone)
        : MeshRenderer(mesh, material), mRootBone{rootBone} {
        mMatrixPalette.resize(MAX_BONE_MATRIX);
    }

    SkinnedMeshRenderer::~SkinnedMeshRenderer() {
    }

    void SkinnedMeshRenderer::setRootBone(GameObject* gameObject) {
        mRootBone = gameObject;
    }

    void SkinnedMeshRenderer::update(real deltaTime) {
        ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
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