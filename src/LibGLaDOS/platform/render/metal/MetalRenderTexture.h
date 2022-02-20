#ifndef GLADOS_METALRENDERTEXTURE_H
#define GLADOS_METALRENDERTEXTURE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/RenderTexture.h"
#include "MetalTextureBase.h"

namespace GLaDOS {
    class MetalRenderTexture : public RenderTexture, public MetalTextureBase {
      public:
        MetalRenderTexture(PixelFormat format);
        ~MetalRenderTexture() override;

        bool generateTexture() override;
        id<MTLSamplerState> metalSamplerState() override;

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  //GLADOS_METALRENDERTEXTURE_H

#endif