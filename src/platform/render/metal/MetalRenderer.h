#ifndef GAMEENGINE_METALRENDERER_H
#define GAMEENGINE_METALRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include "platform/render/Renderer.h"

namespace GameEngine {
  class MetalRenderer : public Renderer {
  public:
    MetalRenderer();
    virtual ~MetalRenderer();

    bool initialize() override;
    void render() const override;

    id<MTLDevice> getMetalDevice() const;
    id<MTLCommandQueue> getMetalCommandQueue() const;
    CAMetalLayer* getMetalLayer() const;
    static MetalRenderer* getInstance();

  private:
    static MetalRenderer* instance;

    id<MTLDevice> mMetalDevice;
    id<MTLCommandQueue> mMetalCommandQueue;
    id<MTLRenderCommandEncoder> mMetalRenderCommandEncoder;
    CAMetalLayer* mMetalLayer{nullptr};
  };
}  // namespace GameEngine

#endif

#endif
