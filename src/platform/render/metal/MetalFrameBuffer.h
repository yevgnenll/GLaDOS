#ifndef GLADOS_METALFRAMEBUFFER_H
#define GLADOS_METALFRAMEBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/FrameBuffer.h"
#include "utils/Semaphore.h"

namespace GLaDOS {
  class MetalRenderer;
  class MetalFrameBuffer : public FrameBuffer {
  public:
    MetalFrameBuffer();
    ~MetalFrameBuffer() override;

    void begin() override;
    void end() override;
    void makeDepthStencilTexture() override;

  private:
    id<MTLCommandQueue> mCommandQueue{nil};
    id<MTLCommandBuffer> mCommandBuffer{nil};
    id<MTLRenderCommandEncoder> mCommandEncoder{nil};
    id<CAMetalDrawable> mNextDrawable{nil};
    id<MTLTexture> mDepthStencilTexture{nil};
    MetalRenderer* mRenderer;
    Semaphore mSemaphore{1};
  };
}  // namespace GLaDOS

#endif

#endif