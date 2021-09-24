#include "Sprite.h"
#include "platform/render/Texture2D.h"
#include <utility>

namespace GLaDOS {
    Sprite::Sprite(Texture2D* texture) : mTexture{texture}, mTextureCoords{0, 1, 1, 0} {
    }

    Sprite::Sprite(Texture2D* texture, Rect<float> textureCoords) : mTexture{texture}, mTextureCoords{std::move(textureCoords)} {
    }

    Rect<float> Sprite::getTextureCoords() const {
        return mTextureCoords;
    }

    Vec2 Sprite::getPivot() const {
        return mPivot;
    }
}  // namespace GLaDOS