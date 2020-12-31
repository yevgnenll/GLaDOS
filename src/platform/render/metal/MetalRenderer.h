#ifndef GAMEENGINE_METALRENDERER_H
#define GAMEENGINE_METALRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/Renderer.h"
#import <Metal/Metal.h>

namespace GameEngine {
  class MetalRenderer : public Renderer {
  public:
    MetalRenderer();
    virtual ~MetalRenderer();

    bool initialize() override;
    void render() const override;

  private:
    id<MTLDevice> mMetalDevice;
    id<MTLCommandQueue> mMetalCommandQueue;
  };
}  // namespace GameEngine

#endif

#endif
