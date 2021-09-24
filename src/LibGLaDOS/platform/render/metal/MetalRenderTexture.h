#ifndef GLADOS_METALRENDERTEXTURE_H
#define GLADOS_METALRENDERTEXTURE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/RenderTexture.h"

namespace GLaDOS {
    class MetalRenderTexture : public RenderTexture {
      public:
        MetalRenderTexture(PixelFormat format);
        ~MetalRenderTexture() override;

      private:
    };
}  // namespace GLaDOS

#endif  //GLADOS_METALRENDERTEXTURE_H

#endif