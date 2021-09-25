#include "Sprite.h"
#include "platform/render/Texture2D.h"
#include <utility>

namespace GLaDOS {
    Sprite::Sprite(Texture2D* texture)
        : mTexture{texture}, mTextureCoords{0, 1, 1, 0}, mPivot{0.5, 0.5} {
    }

    Sprite::Sprite(Texture2D* texture, Rect<float> textureCoords)
        : mTexture{texture}, mTextureCoords{std::move(textureCoords)} {
        mPivot = Vec2{CAST(real, (mTextureCoords.right - mTextureCoords.left) * 0.5),
                      CAST(real, (mTextureCoords.top - mTextureCoords.bottom) * 0.5)};
    }

    Texture2D* Sprite::getTexture() const {
        return mTexture;
    }

    Rect<float> Sprite::getTextureCoords() const {
        return mTextureCoords;
    }

    Vec2 Sprite::getPivot() const {
        return mPivot;
    }
}  // namespace GLaDOS