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
        createRenderable(mRect, mTexture);
    }

    Sprite::Sprite(Texture2D* texture, const Rect<uint32_t>& rectInPixel)
        : mTexture{texture} {
        mRect = normalizePixelRect(rectInPixel, texture->getWidth(), texture->getHeight());
        createRenderable(mRect, mTexture);
    }

    Sprite::Sprite(Texture2D* texture, const Rect<uint32_t>& rectInPixel, int pixelPerUnit)
        : mTexture{texture}, mPixelPerUnit{pixelPerUnit} {
        mRect = normalizePixelRect(rectInPixel, texture->getWidth(), texture->getHeight());
        createRenderable(mRect, mTexture);
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

    Rect<real> Sprite::normalizePixelRect(const Rect<uint32_t>& rectInPixel, uint32_t width, uint32_t height) {
        /*
          rectInPixel as User's perspective
          (0,1)          (1,1) == (w,h)
            +-------------+
            |             |
            |             |
            |             |
            |             |
            +-------------+
          (0,0) == (x,y) (1,0)
        */
        Rect<real> result;

        real invw = 1 / static_cast<real>(width);
        real invh = 1 / static_cast<real>(height);
        uint32_t inverseTop = height - rectInPixel.y; // upside down height

        result.x = static_cast<real>(rectInPixel.x) * invw;
        result.y = static_cast<real>(inverseTop) * invh;
        result.w = (static_cast<real>(rectInPixel.w + rectInPixel.x)) * invw;
        result.h = (static_cast<real>(inverseTop - rectInPixel.h)) * invh;

        return result;
    }

    bool Sprite::createRenderable(const Rect<real>& normalizedRect, Texture2D* texture2D) {
        Mesh* mesh = MeshGenerator::generateRectangle(normalizedRect);
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
        material->setTexture0(texture2D);

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "Sprite initialize failed!");
            return false;
        }

        mRenderable = renderable;

        return true;
    }
}  // namespace GLaDOS