#ifndef GLADOS_CUBEMAPRENDERER_H
#define GLADOS_CUBEMAPRENDERER_H

#include "core/Component.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class TextureCube;
  class CubemapRenderer : public Component {
  public:
    CubemapRenderer();
    ~CubemapRenderer() override;

  private:
    void update(real deltaTime) override;
    void render() override;

    TextureCube* mCubeTexture[static_cast<int>(CubeMapFace::TheNumberOfFace)] = {
        nullptr,
    };
  };
}  // namespace GLaDOS

#endif  //GLADOS_CUBEMAPRENDERER_H
