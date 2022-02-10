#include "MeshRenderer.h"

#include "platform/Platform.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/RenderState.h"
#include "core/GameObject.hpp"
#include "core/component/Camera.h"
#include "core/component/Transform.h"
#include "core/Scene.h"

namespace GLaDOS {
    Logger* MeshRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MeshRenderer");
    MeshRenderer::MeshRenderer() {
        mName = "MeshRenderer";
    }

    MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) {
        mName = "MeshRenderer";
        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "MeshRenderer initialize failed!");
        }
        mRenderable = renderable;
    }

    MeshRenderer::~MeshRenderer() {
        DELETE_T(mRenderable, Renderable);
    }

    void MeshRenderer::update(real deltaTime) {
        if (mRenderable != nullptr) {
            ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
            Camera* mainCamera = mGameObject->scene()->getMainCamera();
            FillMode fillMode = mRenderable->getMaterial()->getShaderProgram()->rasterizerState()->mRasterizerDescription.mFillMode;
            Mat4<real> model = mGameObject->transform()->localToWorldMatrix();
            Mat4<real> modelView = model * mainCamera->worldToCameraMatrix();

            shaderProgram->setUniform("model", model);
            shaderProgram->setUniform("modelViewProj", modelView * mainCamera->projectionMatrix());
            shaderProgram->setUniform("transInvModelView", Mat4<real>::transpose(Mat4<real>::inverse(modelView)));
            shaderProgram->setUniform("viewPos", mainCamera->gameObject()->transform()->position());
            shaderProgram->setUniform("isWireFrameMode", fillMode == FillMode::Lines);
        }
    }

    void MeshRenderer::render() {
        Scene* currentScene = mGameObject->scene();
        Camera* mainCamera = currentScene->getMainCamera();
        BitMask* cullingMask = mainCamera->cullingMask();
        if (cullingMask->isSet(mGameObject->getLayer())) {
            Platform::getRenderer().render(mRenderable, mainCamera->getViewportRect());
        }
    }

    Component* MeshRenderer::clone() {
        MeshRenderer* meshRenderer = NEW_T(MeshRenderer);
        meshRenderer->mIsActive = mIsActive;
        meshRenderer->mRenderable = Platform::getRenderer().createRenderable(NEW_T(Mesh(*mRenderable->getMesh())), NEW_T(Material(*mRenderable->getMaterial())));
        return meshRenderer;
    }
}  // namespace GLaDOS