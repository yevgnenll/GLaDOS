#include "MetalFrameBuffer.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  MetalFrameBuffer::MetalFrameBuffer() : FrameBuffer{}, mRenderer{MetalRenderer::getInstance()} {
    id<MTLDevice> device = mRenderer->getDevice();
    if (nullptr != device) {
      mCommandQueue = [device newCommandQueue];
    }
  }

  void MetalFrameBuffer::begin() {
    CAMetalLayer* metalLayer = mRenderer->getMetalLayer();
    mNextDrawable = [metalLayer nextDrawable];
    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = mNextDrawable.texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(static_cast<double>(mClearColor.r),
                                                                      static_cast<double>(mClearColor.g),
                                                                      static_cast<double>(mClearColor.b),
                                                                      static_cast<double>(mClearColor.a));
    mCommandBuffer = [mCommandQueue commandBuffer];
    mCommandEncoder = [mCommandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    mRenderer->setCommandEncoder(mCommandEncoder);
  }

  void MetalFrameBuffer::end() {
    [mCommandEncoder endEncoding];
    [mCommandBuffer presentDrawable:mNextDrawable];
    [mCommandBuffer commit];
  }
}

#endif