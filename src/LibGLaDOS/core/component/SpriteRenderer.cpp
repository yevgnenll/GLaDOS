#include "SpriteRenderer.h"

#include "platform/Platform.h"
#include "platform/render/IndexBuffer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/Texture2D.h"
#include "platform/render/VertexBuffer.h"
#include "core/Sprite.h"
#include "utils/MeshGenerator.h"
#include "core/GameObject.hpp"
#include "Transform.h"
#include "Camera.h"
#include "core/Scene.h"

namespace GLaDOS {
    Logger* SpriteRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("SpriteRenderer");
    SpriteRenderer::SpriteRenderer() {
        mName = "SpriteRenderer";
    }

    SpriteRenderer::SpriteRenderer(Sprite* sprite) : mSprite{sprite} {
        mName = "SpriteRenderer";

        Mesh* mesh = MeshGenerator::generateRectangle(mSprite->getRect());
        if (mesh == nullptr) {
            LOG_ERROR(logger, "SpriteRenderer initialize failed!");
            return;
        }
        ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgramFromFile("spriteVertex", "spriteFragment");
        if (shaderProgram == nullptr) {
            LOG_ERROR(logger, "SpriteRenderer initialize failed!");
            return;
        }

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        material->setTexture0(mSprite->getTexture());

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "SpriteRenderer initialize failed!");
            return;
        }
        mRenderable = renderable;
    }

    SpriteRenderer::~SpriteRenderer() {
        DELETE_T(mSprite, Sprite);
    }

    void SpriteRenderer::setSprite(Sprite* sprite) {
        mSprite = sprite;
    }

    void SpriteRenderer::setColor(const Color& color) {
        mColor = color;
    }

    void SpriteRenderer::setColorKey(const Color& colorKey) {
        mColorKey = colorKey;
    }

    void SpriteRenderer::setFlipX(bool flipX) {
        mFlipX = flipX;
    }

    void SpriteRenderer::setFlipY(bool flipY) {
        mFlipY = flipY;
    }

    void SpriteRenderer::setUseColorKey(bool useColorKey) {
        mUseColorKey = useColorKey;
    }

    Sprite* SpriteRenderer::getSprite() const {
        return mSprite;
    }

    Color SpriteRenderer::getColor() const {
        return mColor;
    }

    Color SpriteRenderer::getColorKey() const {
        return mColorKey;
    }

    bool SpriteRenderer::getFlipX() const {
        return mFlipX;
    }

    bool SpriteRenderer::getFlipY() const {
        return mFlipY;
    }

    bool SpriteRenderer::isUseColorKey() const {
        return mUseColorKey;
    }

    void SpriteRenderer::update(real deltaTime) {
        ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
        Scene* currentScene = mGameObject->scene();
        Camera* mainCamera = currentScene->getMainCamera();
        Transform* transform = mGameObject->transform();

        // calculate pixel per unit scale value of sprite
        real reversePixelPerUnit = 1 / static_cast<real>(mSprite->getPixelPerUnit());
        real scaleX = static_cast<real>(mSprite->getTexture()->getWidth()) * reversePixelPerUnit;
        real scaleY = static_cast<real>(mSprite->getTexture()->getHeight()) * reversePixelPerUnit;
        transform->setLossyScale({scaleX, scaleY, 1});

        shaderProgram->setUniform("model", transform->localToWorldMatrix());
        shaderProgram->setUniform("view", mainCamera->worldToCameraMatrix());
        shaderProgram->setUniform("projection", mainCamera->projectionMatrix());
        shaderProgram->setUniform("flipX", mFlipX);
        shaderProgram->setUniform("flipY", mFlipY);
        shaderProgram->setUniform("color", mColor);
        shaderProgram->setUniform("colorKey", mColorKey);
        shaderProgram->setUniform("useColorKey", mUseColorKey);
    }

    void SpriteRenderer::render() {
        Platform::getRenderer().render(mRenderable);
    }
}  // namespace GLaDOS