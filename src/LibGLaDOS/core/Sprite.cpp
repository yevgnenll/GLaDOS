#include "Sprite.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/ShaderProgram.h"
#include "utils/MeshGenerator.h"
#include "platform/Platform.h"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
    Logger* Sprite::logger = LoggerRegistry::getInstance().makeAndGetLogger("Sprite");
    Sprite::Sprite(Texture2D* texture)
        : mTexture{texture}, mRect{0, 0, 1, 1} {
        createRenderable();
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect)
        : mTexture{texture}, mRect{rect} {
        createRenderable();
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect, int pixelPerUnit)
        : mTexture{texture}, mRect{rect}, mPixelPerUnit{pixelPerUnit} {
        createRenderable();
    }

    Texture2D* Sprite::getTexture() const {
        return mTexture;
    }

    Rect<float> Sprite::getRect() const {
        return mRect;
    }

    Renderable* Sprite::getRenderable() {
        return mRenderable;
    }

    int Sprite::getPixelPerUnit() const {
        return mPixelPerUnit;
    }

    void Sprite::setPixelPerUnit(int ppu) {
        mPixelPerUnit = ppu;
    }

    bool Sprite::createRenderable() {
        Mesh* mesh = MeshGenerator::generateRectangle(getRect());
        if (mesh == nullptr) {
            LOG_ERROR(logger, "Sprite initialize failed!");
            return false;
        }
        ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgramFromFile("spriteVertex", "spriteFragment");
        if (shaderProgram == nullptr) {
            LOG_ERROR(logger, "Sprite initialize failed!");
            return false;
        }

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        material->setTexture0(getTexture());

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "Sprite initialize failed!");
            return false;
        }

        mRenderable = renderable;

        return true;
    }
}  // namespace GLaDOS