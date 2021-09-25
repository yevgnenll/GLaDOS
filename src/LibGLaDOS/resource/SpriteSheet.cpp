#include "SpriteSheet.h"

#include <utility>

#include "Sprite.h"
#include "math/Rect.hpp"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
    SpriteSheet::SpriteSheet(Texture2D* texture, Sizei spriteSize, std::size_t numSprites, Sizei spacing)
        : mTexture{texture}, mSpriteSize{std::move(spriteSize)}, mSpriteSpacing{std::move(spacing)}, mSpriteCount{numSprites} {
        createSpriteSheet();
    }

    SpriteSheet::~SpriteSheet() {
        deallocIterable(mSprites);
    }

    void SpriteSheet::createSpriteSheet() {
        /* SpriteSheet
         *  0 -> 1 -> 2 -> 3
         *  4 -> 5 -> 6 -> 7
         *  top-left is 0, bottom-right is end
        */
        std::size_t currentX = 0;
        std::size_t currentY = mTexture->getHeight() - mSpriteSize.y;
        std::size_t stepX = mSpriteSize.x + mSpriteSpacing.x;
        std::size_t stepY = mSpriteSize.y + mSpriteSpacing.y;
        real sizeX = 1 / CAST(real, mTexture->getWidth());
        real sizeY = 1 / CAST(real, mTexture->getHeight());
        for (std::size_t i = 0; i < mSpriteCount; i++) {
            Rect<real> rect{
                (currentX + mSpriteSize.x) * sizeX,
                currentX * sizeX,
                (currentY + mSpriteSize.y) * sizeY,
                currentY * sizeY
            };

            mSprites.emplace_back(NEW_T(Sprite(mTexture, std::move(rect))));

            currentX += stepX;
            if (currentX >= mTexture->getWidth()) {
                currentX = 0;
                currentY -= stepY;
            }
        }
    }

    Texture2D* SpriteSheet::getTexture() const {
        return mTexture;
    }

    Sprite* SpriteSheet::getSprite(std::size_t index) const {
        return mSprites.at(index);
    }
}  // namespace GLaDOS