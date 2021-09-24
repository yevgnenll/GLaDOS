#ifndef GLADOS_RENDERTEXTURE_H
#define GLADOS_RENDERTEXTURE_H

#include "Texture.h"

namespace GLaDOS {
    class RenderTexture : public Texture {
      public:
        RenderTexture(PixelFormat format);
        ~RenderTexture() override;

        bool loadTextureFromFile() override { return false; }
        bool loadTextureFromBuffer(StreamBuffer& buffer) override { return false; }

      private:
    };
}  // namespace GLaDOS

#endif  //GLADOS_RENDERTEXTURE_H
