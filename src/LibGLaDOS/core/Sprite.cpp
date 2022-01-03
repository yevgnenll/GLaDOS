#include "Sprite.h"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
    Sprite::Sprite(Texture2D* texture)
        : mTexture{texture}, mRect{0, 0, 1, 1} {
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect)
        : mTexture{texture}, mRect{rect} {
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect, int pixelPerUnit)
        : mTexture{texture}, mRect{rect}, mPixelPerUnit{pixelPerUnit} {
    }

    Texture2D* Sprite::getTexture() const {
        return mTexture;
    }

    Rect<float> Sprite::getRect() const {
        return mRect;
    }

    int Sprite::getPixelPerUnit() const {
        return mPixelPerUnit;
    }

    void Sprite::setPixelPerUnit(int ppu) {
        mPixelPerUnit = ppu;
    }
}  // namespace GLaDOS