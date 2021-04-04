#include "MetalTextureBase.h"

#ifdef PLATFORM_MACOS

#include "utils/Enumeration.h"

namespace GLaDOS {
  MetalTextureBase::~MetalTextureBase() {
    deallocate();
  }

  id<MTLTexture> MetalTextureBase::texture() {
    return mTexture;
  }

  MTLPixelFormat MetalTextureBase::mapMetalPixelFormatFrom(PixelFormat format) {
    switch (format) {
      case PixelFormat::Red8:
        return MTLPixelFormatR8Unorm;
      case PixelFormat::Red16:
        return MTLPixelFormatR16Unorm;
      case PixelFormat::RedHalf:
        return MTLPixelFormatR16Float;
      case PixelFormat::RedFloat:
        return MTLPixelFormatR32Float;
      case PixelFormat::RG16:
        return MTLPixelFormatRG8Unorm;
      case PixelFormat::RG32:
        return MTLPixelFormatRG16Unorm;
      case PixelFormat::RGHalf:
        return MTLPixelFormatRG16Float;
      case PixelFormat::RGFloat:
        return MTLPixelFormatRG32Float;
      case PixelFormat::RGB24:  // fallback alpha with 0xFF
      case PixelFormat::RGBA32:
        return MTLPixelFormatRGBA8Unorm;
      case PixelFormat::BGRA32:
        return MTLPixelFormatBGRA8Unorm;
      case PixelFormat::RGBA64:
        return MTLPixelFormatRGBA16Unorm;
      case PixelFormat::RGBAHalf:
        return MTLPixelFormatRGBA16Float;
      case PixelFormat::RGBAFloat:
        return MTLPixelFormatRGBA32Float;
      case PixelFormat::Alpha8:
        return MTLPixelFormatA8Unorm;
      case PixelFormat::sRGB24:  // fallback alpha with 0xFF
      case PixelFormat::sRGBA32:
        return MTLPixelFormatRGBA8Unorm_sRGB;
      case PixelFormat::Depth32:
        return MTLPixelFormatDepth32Float;
      case PixelFormat::Stencil8:
        return MTLPixelFormatStencil8;
      case PixelFormat::Depth24Stencil8:
        return MTLPixelFormatX24_Stencil8;  // MTLPixelFormatDepth24Unorm_Stencil8 can't directly read the stencil value of a texture
      case PixelFormat::Depth32Stencil8:
        return MTLPixelFormatDepth32Float_Stencil8;
      default:
        LOG_ERROR("default", "Invalid Texture format.");
        break;
    }

    return MTLPixelFormatInvalid;
  }

  MTLTextureUsage MetalTextureBase::mapMetalTextureUsageFrom(TextureUsage usage) {
    switch (usage) {
      case TextureUsage::ShaderRead:
        return MTLTextureUsageShaderRead;
      case TextureUsage::ShaderWrite:
        return MTLTextureUsageShaderWrite;
      case TextureUsage::RenderTarget:
        return MTLTextureUsageRenderTarget;
      default:
        break;
    }

    return MTLTextureUsageUnknown;
  }

  void MetalTextureBase::deallocate() {
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