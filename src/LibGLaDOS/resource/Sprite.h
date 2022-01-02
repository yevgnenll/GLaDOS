#ifndef GLADOS_SPRITE_H
#define GLADOS_SPRITE_H

#include "math/Rect.hpp"
#include "math/Point.hpp"

namespace GLaDOS {
    class Texture2D;
    class Sprite {
      public:
        explicit Sprite(Texture2D* texture);
        Sprite(Texture2D* texture, const Rect<real>& rect);
        Sprite(Texture2D* texture, const Rect<real>& rect, const Point<real>& pivot);
        Sprite(Texture2D* texture, const Rect<real>& rect, const Point<real>& pivot, int pixelPerUnit);
        ~Sprite() = default;

        Texture2D* getTexture() const;
        Rect<real> getRect() const;
        Point<real> getPivot() const;
        int getPixelPerUnit() const;

        void setPivot(const Point<real>& pivot);
        void setPixelPerUnit(int ppu);

      private:
        Texture2D* mTexture;
        Rect<real> mRect;
        Point<real> mPivot;
        int mPixelPerUnit{10};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITE_H
