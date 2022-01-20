#include "CubemapRenderer.h"

#include "core/component/Camera.h"
#include "core/GameObject.hpp"
#include "core/Scene.h"
#include "platform/Platform.h"
#include "platform/render/IndexBuffer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/RenderState.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/TextureCube.h"
#include "platform/render/VertexBuffer.h"
#include "utils/MeshGenerator.h"

namespace GLaDOS {
    Logger* CubemapRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("CubemapRenderer");
    CubemapRenderer::CubemapRenderer() {
        mName = "CubemapRenderer";
        mRenderable = CubemapRenderer::createRenderable();
    }

    CubemapRenderer::~CubemapRenderer() {
    }

    void CubemapRenderer::setTextureCube(TextureCube* textureCube) {
        if (mRenderable != nullptr) {
            mRenderable->getMaterial()->setTexture0(textureCube);
        }
    }

    void CubemapRenderer::update(real deltaTime) {
        if (mRenderable != nullptr) {
            ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
            Scene* currentScene = mGameObject->scene();
            Camera* mainCamera = currentScene->getMainCamera();
            // can not be affected by camera orthographic mode
            Mat4<real> viewProjection = Mat4<real>::toMat3(mainCamera->worldToCameraMatrix()) *
                                   Mat4<real>::perspective(Math::toRadians(mainCamera->fieldOfView()),
                                                           mainCamera->aspectRatio(), mainCamera->nearClipPlane(), mainCamera->farClipPlane());
            shaderProgram->setUniform("viewProjection", viewProjection);
        }
    }

    Component* CubemapRenderer::clone() {
        CubemapRenderer* cubemapRenderer = NEW_T(CubemapRenderer);
        cubemapRenderer->mIsActive = mIsActive;
        cubemapRenderer->mRenderable = createRenderable();
        cubemapRenderer->setTextureCube(static_cast<TextureCube*>(mRenderable->getMaterial()->getTexture0()));
        return cubemapRenderer;
    }

    Renderable* CubemapRenderer::createRenderable() {
        Mesh* mesh = MeshGenerator::generateCube();
        if (mesh == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed: generate mesh");
            return nullptr;
        }
        ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgramFromFile("skyboxVertex", "skyboxFragment");
        if (shaderProgram == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed: create shader program");
            return nullptr;
        }
        DepthStencilDescription depthStencilDesc{};
        depthStencilDesc.mIsDepthWriteEnable = false;
        depthStencilDesc.mDepthFunction = ComparisonFunction::LessEqual;
        shaderProgram->setDepthStencilState(depthStencilDesc);

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed: create renderable");
            return nullptr;
        }

        return renderable;
    }
}  // namespace GLaDOS