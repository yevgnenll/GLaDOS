#include "MetalTexture2D.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderState.h"

namespace GLaDOS {
  MetalTexture2D::MetalTexture2D(const std::string& name, TextureFormat format) : Texture2D{name, format} {
  }

  MetalTexture2D::~MetalTexture2D() {
    deallocate();
  }

  id<MTLTexture> MetalTexture2D::texture() {
    return mTexture;
  }

  void MetalTexture2D::replaceRegion(uint32_t x, uint32_t y, uint8_t* data) {
    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nil) {
      LOG_ERROR("Invalid Metal device state, texture creation failed.");
      return;
    }

    deallocate();

    mTextureDescriptor = [MTLTextureDescriptor new];
    mTextureDescriptor.pixelFormat = MetalTexture2D::mapMetalPixelFormatFrom(mFormat);
    mTextureDescriptor.width = mWidth;
    mTextureDescriptor.height = mHeight;
    mTextureDescriptor.usage = MetalTexture2D::mapMetalTextureUsageFrom(mUsage);
    mTextureDescriptor.textureType = MTLTextureType2D;
    mTexture = [device newTextureWithDescriptor:mTextureDescriptor];

    if (mTexture != nil) {
      MTLRegion region = { { x, y, 0}, { mWidth, mHeight, 1} };
      uint32_t bytesPerRow = mChannels * mWidth;
      [mTexture replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:bytesPerRow];
    }
  }

  id<MTLSamplerState> MetalTexture2D::metalSamplerState() {
    return static_cast<MetalSamplerState*>(samplerState())->getMetalSamplerState();
  }

  void MetalTexture2D::deallocate() {
    if(mTexture != nil) {
      [mTexture release];
      mTexture = nil;
    }

    if(mTextureDescriptor != nil) {
      [mTextureDescriptor release];
      mTextureDescriptor = nil;
    }
  }

  constexpr MTLPixelFormat MetalTexture2D::mapMetalPixelFormatFrom(TextureFormat format) {
    switch (format) {
      case TextureFormat::Red8:
        return MTLPixelFormatR8Unorm;
      case TextureFormat::Red16:
        return MTLPixelFormatR16Unorm;
      case TextureFormat::RedHalf:
        return MTLPixelFormatR16Float;
      case TextureFormat::RedFloat:
        return MTLPixelFormatR32Float;
      case TextureFormat::RG16:
        return MTLPixelFormatRG8Unorm;
      case TextureFormat::RG32:
        return MTLPixelFormatRG16Unorm;
      case TextureFormat::RGHalf:
        return MTLPixelFormatRG16Float;
      case TextureFormat::RGFloat:
        return MTLPixelFormatRG32Float;
      case TextureFormat::RGB24: // fallback alpha with 0xFF
      case TextureFormat::RGBA32:
        return MTLPixelFormatRGBA8Unorm;
      case TextureFormat::BGRA32:
        return MTLPixelFormatBGRA8Unorm;
      case TextureFormat::RGBA64:
        return MTLPixelFormatRGBA16Unorm;
      case TextureFormat::RGBAHalf:
        return MTLPixelFormatRGBA16Float;
      case TextureFormat::RGBAFloat:
        return MTLPixelFormatRGBA32Float;
      case TextureFormat::Alpha8:
        return MTLPixelFormatA8Unorm;
      case TextureFormat::sRGB24: // fallback alpha with 0xFF
      case TextureFormat::sRGBA32:
        return MTLPixelFormatRGBA8Unorm_sRGB;
      case TextureFormat::Depth32:
        return MTLPixelFormatDepth32Float;
      case TextureFormat::Stencil8:
        return MTLPixelFormatStencil8;
      case TextureFormat::Depth24Stencil8:
        return MTLPixelFormatX24_Stencil8; // MTLPixelFormatDepth24Unorm_Stencil8 can't directly read the stencil value of a texture
      case TextureFormat::Depth32Stencil8:
        return MTLPixelFormatDepth32Float_Stencil8;
      default:
        LOG_ERROR("Invalid Texture format.");
        break;
    }

    return MTLPixelFormatInvalid;
  }

  constexpr MTLTextureUsage MetalTexture2D::mapMetalTextureUsageFrom(TextureUsage usage) {
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
}

#endif