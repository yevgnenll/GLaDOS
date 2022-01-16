#ifndef GLADOS_CUBEMAPRENDERER_H
#define GLADOS_CUBEMAPRENDERER_H

#include "MeshRenderer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class Logger;
    class TextureCube;
    class CubemapRenderer : public MeshRenderer {
      public:
        CubemapRenderer();
        ~CubemapRenderer() override;

        void setTextureCube(TextureCube* textureCube);

      protected:
        void update(real deltaTime) override;
        Component* clone() override;

      private:
        static Logger* logger;

        static Renderable* createRenderable();
    };
}  // namespace GLaDOS

#endif  //GLADOS_CUBEMAPRENDERER_H
