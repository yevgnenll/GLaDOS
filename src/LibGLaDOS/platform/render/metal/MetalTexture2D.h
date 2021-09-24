#ifndef GLADOS_METALTEXTURE2D_H
#define GLADOS_METALTEXTURE2D_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "MetalTextureBase.h"
#include "platform/render/Texture2D.h"

namespace GLaDOS {
    class Logger;
    class MetalTexture2D : public Texture2D, public MetalTextureBase {
      public:
        MetalTexture2D(const std::string& name, PixelFormat format);
        ~MetalTexture2D() override;

        void generateTexture(uint32_t x, uint32_t y, uint8_t* data) override;
        void replaceRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t level, uint8_t* data) override;
        id<MTLSamplerState> metalSamplerState() override;

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  //GLADOS_METALTEXTURE2D_H

#endif