#ifndef GLADOS_SPRITE_H
#define GLADOS_SPRITE_H

#include "math/Rect.hpp"
#include "math/Vec2.h"

namespace GLaDOS {
    class Texture2D;
    class Sprite {
      public:
        explicit Sprite(Texture2D* texture);
        Sprite(Texture2D* texture, Rect<float> textureCoords);
        ~Sprite() = default;

        Texture2D* getTexture() const;
        Rect<float> getTextureCoords() const;
        Vec2 getPivot() const;

      private:
        Texture2D* mTexture;
        Rect<float> mTextureCoords;
        Vec2 mPivot;
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITE_H