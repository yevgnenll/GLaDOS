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
    Sprite::Sprite() : mTexture{nullptr}, mRect{}, mAnchorPoint{}, mRenderable{nullptr} {
    }

    Sprite::Sprite(Texture2D* texture) : mTexture{texture}, mRect{getFullRect(mTexture)}, mAnchorPoint{getCenterAnchorPoint(mRect)} {
        createRenderable();
    }

    Sprite::Sprite(Texture2D* texture, const Rect<uint32_t>& rect) : mTexture{texture}, mRect{rect}, mAnchorPoint{getCenterAnchorPoint(mRect)} {
        createRenderable();
    }

    Sprite::Sprite(Texture2D* texture, Point<real> anchorPoint) : mTexture{texture}, mRect{getFullRect(mTexture)}, mAnchorPoint{anchorPoint} {
        createRenderable();
    }

    Sprite::Sprite(Texture2D* texture, const Rect<uint32_t>& rect, Point<real> anchorPoint) : mTexture{texture}, mRect{rect}, mAnchorPoint{anchorPoint} {
        createRenderable();
    }

    Sprite::Sprite(const Sprite& other) : mTexture{other.mTexture}, mRect{other.mRect}, mAnchorPoint{other.mAnchorPoint} {
        createRenderable();
    }

    Sprite& Sprite::operator=(const Sprite& other) {
        mTexture = other.mTexture;
        mRect = other.mRect;
        mAnchorPoint = other.mAnchorPoint;
        createRenderable();
        return *this;
    }

    Texture2D* Sprite::getTexture() const {
        return mTexture;
    }

    Rect<uint32_t> Sprite::getRect() const {
        return mRect;
    }

    Rect<real> Sprite::getRectNormalized() const {
        return normalizePixelRect(mRect, mTexture->getWidth(), mTexture->getHeight());
    }

    Point<real> Sprite::getAnchorPoint() const {
        return mAnchorPoint;
    }

    Point<real> Sprite::getAnchorPointNormalized() const {
        return Point<real>{mAnchorPoint.x / static_cast<real>(mRect.w), mAnchorPoint.y / static_cast<real>(mRect.h)};
    }

    Renderable* Sprite::getRenderable() {
        return mRenderable;
    }

    Rect<uint32_t> Sprite::getFullRect(Texture2D* texture2D) {
        return Rect<uint32_t>{0.f, 0.f, texture2D->getWidth(), texture2D->getHeight()};
    }

    Point<real> Sprite::getCenterAnchorPoint(const Rect<uint32_t>& rect) {
        return Point<real>{static_cast<real>(rect.w) * 0.5f, static_cast<real>(rect.h) * 0.5f};
    }

    Rect<real> Sprite::normalizePixelRect(const Rect<uint32_t>& rect, uint32_t width, uint32_t height) {
        /*
          rect as User's perspective
          (0,1)          (1,1) == (w,h)
            +-------------+
            |             |
            |             |
            |             |
            |             |
            +-------------+
          (0,0) == (x,y) (1,0)

          This function convert and normalize its coordinates into downward-y axis.
        */
        Rect<real> result;

        real invw = 1 / static_cast<real>(width);
        real invh = 1 / static_cast<real>(height);
        uint32_t inverseTop = height - rect.y; // upside down height

        result.x = static_cast<real>(rect.x) * invw;
        result.y = static_cast<real>(inverseTop) * invh;
        result.w = (static_cast<real>(rect.w + rect.x)) * invw;
        result.h = (static_cast<real>(inverseTop - rect.h)) * invh;

        return result;
    }

    bool Sprite::createRenderable() {
        Mesh* mesh = MeshGenerator::generateRectangle(getRectNormalized(), Size<uint32_t>(mRect.w, mRect.h));
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
        material->setTexture0(mTexture);

        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "Sprite initialize failed!");
            return false;
        }

        mRenderable = renderable;

        return true;
    }
}  // namespace GLaDOS