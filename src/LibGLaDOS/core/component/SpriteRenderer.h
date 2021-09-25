#ifndef GLADOS_SPRITERENDERER_H
#define GLADOS_SPRITERENDERER_H

#include "MeshRenderer.h"
#include "math/Color.h"

namespace GLaDOS {
    class Sprite;
    class SpriteRenderer : public MeshRenderer {
      public:
        SpriteRenderer();
        explicit SpriteRenderer(Sprite* sprite);
        ~SpriteRenderer() override;

        void setSprite(Sprite* sprite);
        void setColor(const Color& color);
        void setColorKey(const Color& colorKey);
        void setFlipX(bool flipX);
        void setFlipY(bool flipY);
        void setUseColorKey(bool useColorKey);

        Sprite* getSprite() const;
        Color getColor() const;
        Color getColorKey() const;
        bool getFlipX() const;
        bool getFlipY() const;
        bool isUseColorKey() const;

      protected:
        void update(real deltaTime) override;
        void render() override;

      private:
        static Logger* logger;

        Sprite* mSprite{nullptr};
        Color mColor;
        Color mColorKey;
        bool mFlipX{false};
        bool mFlipY{false};
        bool mUseColorKey{false};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITERENDERER_H
