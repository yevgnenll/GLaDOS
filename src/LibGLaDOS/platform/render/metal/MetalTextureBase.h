#ifndef GLADOS_METALTEXTUREBASE_H
#define GLADOS_METALTEXTUREBASE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
    class Logger;
    class MetalTextureBase {
      public:
        MetalTextureBase() = default;
        virtual ~MetalTextureBase();

        id<MTLTexture> texture();
        virtual id<MTLSamplerState> metalSamplerState() = 0;

      protected:
        void release();

        MTLTextureDescriptor* mTextureDescriptor{nil};
        id<MTLTexture> mTexture{nil};

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif

#endif