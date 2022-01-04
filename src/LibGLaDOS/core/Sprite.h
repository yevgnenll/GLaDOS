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
        Sprite(Texture2D* texture, const Rect<uint32_t>& rectInPixel, Point<real> pivotInPixel);
        ~Sprite() = default;

        Texture2D* getTexture() const;
        Rect<real> getRect() const;
        Point<real> getPivot() const;
        Renderable* getRenderable();

      private:
        static Rect<real> normalizePixelRect(const Rect<uint32_t>& rectInPixel, uint32_t width, uint32_t height);
        static Point<real> normalizePixelPoint(const Point<real>& pointInPixel, uint32_t width, uint32_t height);
        bool createRenderable(const Rect<real>& normalizedRect, const Rect<uint32_t>& rectInPixel, Texture2D* texture2D, Point<real> pivot);

        static Logger* logger;

        Texture2D* mTexture;
        Rect<real> mRect; // normalized texture rect
        Point<real> mPivot; // normalized pivot
        Renderable* mRenderable{nullptr};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITE_H
