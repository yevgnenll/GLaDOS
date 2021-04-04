#ifndef GLADOS_METALTEXTURECUBE_H
#define GLADOS_METALTEXTURECUBE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "MetalTextureBase.h"
#include "platform/render/TextureCube.h"

namespace GLaDOS {
  class MetalTextureCube : public TextureCube, public MetalTextureBase {
  public:
    MetalTextureCube(const std::string& name, PixelFormat format);

    id<MTLSamplerState> metalSamplerState() override;
    void generateTexture(Vector<uint8_t*> data) override;
    void replaceRegion(uint32_t size, uint32_t slice, uint32_t bytesPerRow, uint32_t bytesPerImage, uint8_t* data) override;
  };
}  // namespace GLaDOS

#endif

#endif