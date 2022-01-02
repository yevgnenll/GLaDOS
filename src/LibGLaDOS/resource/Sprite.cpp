#include "Sprite.h"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
    Sprite::Sprite(Texture2D* texture)
        : mTexture{texture}, mRect{0, 0, 1, 1}, mPivot{0.5, 0.5} {
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect)
        : mTexture{texture}, mRect{rect} {
        mPivot = Point<real>{
            CAST(real, (mRect.right - mRect.left) * 0.5),
            CAST(real, (mRect.bottom - mRect.top) * 0.5)
        };
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect, const Point<real>& pivot)
        : mTexture{texture}, mRect{rect}, mPivot{pivot} {
    }

    Sprite::Sprite(Texture2D* texture, const Rect<real>& rect, const Point<real>& pivot, int pixelPerUnit)
        : mTexture{texture}, mRect{rect}, mPivot{pivot}, mPixelPerUnit{pixelPerUnit} {
    }

    Texture2D* Sprite::getTexture() const {
        return mTexture;
    }

    Rect<float> Sprite::getRect() const {
        return mRect;
    }

    Point<real> Sprite::getPivot() const {
        return mPivot;
    }

    int Sprite::getPixelPerUnit() const {
        return mPixelPerUnit;
    }

    void Sprite::setPivot(const Point<real>& pivot) {
        mPivot = pivot;
    }

    void Sprite::setPixelPerUnit(int ppu) {
        mPixelPerUnit = ppu;
    }
}  // namespace GLaDOS