#include "SpriteSheet.h"

#include "Sprite.h"
#include "math/Rect.hpp"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
  SpriteSheet::SpriteSheet(Texture2D* texture, Sizei spriteSize, std::size_t numSprites, Sizei spacing)
      : mTexture{texture}, mSpriteSize{spriteSize}, mSpriteSpacing{spacing}, mSpriteCount{numSprites} {
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
    for (std::size_t i = 0; i < mSpriteCount; i++) {
      Rect<float> rect{
          (currentX + mSpriteSize.x) / CAST(float, mTexture->getWidth()),
          currentX / CAST(float, mTexture->getWidth()),
          (currentY + mSpriteSize.y) / CAST(float, mTexture->getHeight()),
          currentY / CAST(float, mTexture->getHeight())};

      mSprites.emplace_back(NEW_T(Sprite(mTexture, rect)));

      currentX += mSpriteSize.x + mSpriteSpacing.x;
      if (currentX >= mTexture->getWidth()) {
        currentX = 0;
        currentY -= mSpriteSize.y + mSpriteSpacing.y;
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