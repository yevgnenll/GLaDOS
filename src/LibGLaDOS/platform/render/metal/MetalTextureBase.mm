#include "MetalTextureBase.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
    Logger* MetalTextureBase::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalTextureBase");

    MetalTextureBase::~MetalTextureBase() {
        release();
    }

    id<MTLTexture> MetalTextureBase::texture() {
        return mTexture;
    }

    void MetalTextureBase::release() {
        if (mTexture != nil) {
            [mTexture release];
            mTexture = nil;
        }

        if (mTextureDescriptor != nil) {
            [mTextureDescriptor release];
            mTextureDescriptor = nil;
        }
    }
}

#endif