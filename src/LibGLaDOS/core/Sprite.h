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
        Sprite();
        explicit Sprite(Texture2D* texture);
        Sprite(Texture2D* texture, const Rect<uint32_t>& rect);
        Sprite(Texture2D* texture, Point<real> anchorPoint);
        Sprite(Texture2D* texture, const Rect<uint32_t>& rect, Point<real> anchorPoint);
        ~Sprite() = default;

        Sprite(const Sprite& other);
        Sprite& operator=(const Sprite& other);

        Texture2D* getTexture() const;
        Rect<uint32_t> getRect() const;
        Rect<real> getRectNormalized() const;
        Point<real> getAnchorPoint() const;
        Point<real> getAnchorPointNormalized() const;
        Renderable* getRenderable();

        static Rect<uint32_t> getFullRect(Texture2D* texture2D);
        static Point<real> getCenterAnchorPoint(const Rect<uint32_t>& rect);
        static Rect<real> normalizePixelRect(const Rect<uint32_t>& rect, uint32_t width, uint32_t height);

      private:
        bool createRenderable();

        static Logger* logger;

        Texture2D* mTexture;
        Rect<uint32_t> mRect;
        Point<real> mAnchorPoint;
        Renderable* mRenderable{nullptr};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITE_H
