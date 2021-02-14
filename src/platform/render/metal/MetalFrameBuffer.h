#ifndef GLADOS_METALFRAMEBUFFER_H
#define GLADOS_METALFRAMEBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/FrameBuffer.h"

namespace GLaDOS {
  class MetalRenderer;
  class MetalFrameBuffer : public FrameBuffer {
  public:
    MetalFrameBuffer();
    ~MetalFrameBuffer() override = default;

    void begin() override;
    void end() override;

  private:
    id<MTLCommandQueue> mCommandQueue{nil};
    id<MTLCommandBuffer> mCommandBuffer{nil};
    id<MTLRenderCommandEncoder> mCommandEncoder{nil};
    id<CAMetalDrawable> mNextDrawable{nil};
    MetalRenderer* mRenderer;
  };
}  // namespace GLaDOS

#endif

#endif