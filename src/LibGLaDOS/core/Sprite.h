#ifndef GLADOS_SPRITE_H
#define GLADOS_SPRITE_H

#include "math/Rect.hpp"
#include "math/Point.hpp"

namespace GLaDOS {
    class Texture2D;
    class Renderable;
    class Logger;
    class Sprite {
      public:
        explicit Sprite(Texture2D* texture);
        Sprite(Texture2D* texture, const Rect<uint32_t>& rectInPixel);
        Sprite(Texture2D* texture, const Rect<uint32_t>& rectInPixel, int pixelPerUnit);
        ~Sprite() = default;

        Texture2D* getTexture() const;
        Rect<real> getRect() const;
        Renderable* getRenderable();
        int getPixelPerUnit() const;
        void setPixelPerUnit(int ppu);

      private:
        static Rect<real> normalizePixelRect(const Rect<uint32_t>& rectInPixel, uint32_t width, uint32_t height) ;
        bool createRenderable(const Rect<real>& normalizedRect, Texture2D* texture2D);

        static Logger* logger;

        Texture2D* mTexture;
        Rect<real> mRect;
        int mPixelPerUnit{10};
        Point<real> mPivot;
        Renderable* mRenderable{nullptr};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITE_H
