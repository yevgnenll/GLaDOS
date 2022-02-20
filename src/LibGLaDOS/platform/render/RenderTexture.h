#ifndef GLADOS_RENDERTEXTURE_H
#define GLADOS_RENDERTEXTURE_H

#include "Texture.h"

namespace GLaDOS {
    class RenderTexture : public Texture {
      public:
        RenderTexture(PixelFormat format);
        ~RenderTexture() override;

        virtual bool generateTexture() = 0;
    };
}  // namespace GLaDOS

#endif  //GLADOS_RENDERTEXTURE_H
