#ifndef GLADOS_SPRITERENDERER_H
#define GLADOS_SPRITERENDERER_H

#include "MeshRenderer.h"
#include "math/Color.h"

namespace GLaDOS {
    class Sprite;
    class SpriteRenderer : public MeshRenderer {
      public:
        SpriteRenderer();
        SpriteRenderer(Sprite* sprite);
        ~SpriteRenderer() override;

        void setSprite(Sprite* sprite);
        void setColor(const Color& color);
        void setFlipX(bool flipX);
        void setFlipY(bool flipY);

        Sprite* getSprite() const;
        Color getColor() const;
        bool getFlipX() const;
        bool getFlipY() const;

      protected:
        void update(real deltaTime) override;
        void render() override;

      private:
        static Logger* logger;

        Sprite* mSprite{nullptr};
        Color mColor;
        bool mFlipX{false};
        bool mFlipY{false};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SPRITERENDERER_H
