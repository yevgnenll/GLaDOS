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

      private:
        void update(real deltaTime) override;
        void render() override;

        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  //GLADOS_CUBEMAPRENDERER_H
