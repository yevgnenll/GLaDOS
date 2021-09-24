#ifndef GLADOS_SPRITESHEET_H
#define GLADOS_SPRITESHEET_H

#include "math/Size.hpp"
#include "utils/Stl.h"

namespace GLaDOS {
    class Texture2D;
    class Sprite;
    class SpriteSheet {
      public:
        SpriteSheet(Texture2D* texture, Sizei spriteSize, std::size_t numSprites, Sizei spacing = Size<std::size_t>(0, 0));
        ~SpriteSheet();

        Texture2D* getTexture() const;
        Sprite* getSprite(std::size_t index) const;

      private:
        void createSpriteSheet();

        Texture2D* mTexture{nullptr};
        Vector<Sprite*> mSprites;
        Sizei mSpriteSize;
        Sizei mSpriteSpacing;
        std::size_t mSpriteCount{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITESHEET_H
