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

        Mesh* mesh = MeshGenerator::generateCube();
        if (mesh == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed!");
            return;
        }
        ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgramFromFile("skyboxVertex", "skyboxFragment");
        if (shaderProgram == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed!");
            return;
        }
        DepthStencilDescription depthStencilDesc{};
        depthStencilDesc.mIsDepthWriteEnable = false;
        depthStencilDesc.mDepthFunction = ComparisonFunction::LessEqual;
        shaderProgram->setDepthStencilState(depthStencilDesc);

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "CubemapRenderer initialize failed!");
            return;
        }
        mRenderable = renderable;
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
            Mat4x viewProjection = Mat4x::toMat3(mainCamera->worldToCameraMatrix()) *
                                   Mat4<real>::perspective(Math::toRadians(mainCamera->fieldOfView()),
                                                           mainCamera->aspectRatio(), mainCamera->nearClipPlane(), mainCamera->farClipPlane());
            shaderProgram->setUniform("viewProjection", viewProjection);
        }
    }
}  // namespace GLaDOS