#include "MetalFrameBuffer.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  MetalFrameBuffer::MetalFrameBuffer() : FrameBuffer{}, mRenderer{MetalRenderer::getInstance()} {
    id<MTLDevice> device = mRenderer->getDevice();
    if (nullptr != device) {
      mCommandQueue = [device newCommandQueue];
    }
  }

  MetalFrameBuffer::~MetalFrameBuffer() {
    [mDepthStencilTexture release];
    [mCommandQueue release];
  }

  void MetalFrameBuffer::begin() {
    // https://developer.apple.com/documentation/metal/synchronization/synchronizing_cpu_and_gpu_work?language=objc
    mSemaphore.wait();
    CAMetalLayer* metalLayer = mRenderer->getMetalLayer();
    mNextDrawable = [metalLayer nextDrawable];
    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    MTLRenderPassColorAttachmentDescriptor* colorAttachment = passDescriptor.colorAttachments[0];

    // color, depth, stencil parameter setting
    colorAttachment.texture = mNextDrawable.texture;
    colorAttachment.clearColor = MTLClearColorMake(static_cast<double>(mClearColor.r),
                                                   static_cast<double>(mClearColor.g),
                                                   static_cast<double>(mClearColor.b),
                                                   static_cast<double>(mClearColor.a));
    colorAttachment.storeAction = MTLStoreActionStore;
    colorAttachment.loadAction = MTLLoadActionClear;

    MTLRenderPassDepthAttachmentDescriptor* depthAttachment = passDescriptor.depthAttachment;
    depthAttachment.texture = mDepthStencilTexture;
    depthAttachment.clearDepth = 1.0;
    depthAttachment.storeAction = MTLStoreActionDontCare;
    depthAttachment.loadAction = MTLLoadActionClear;

    MTLRenderPassStencilAttachmentDescriptor* stencilAttachment = passDescriptor.stencilAttachment;
    stencilAttachment.texture = mDepthStencilTexture;
    stencilAttachment.storeAction = MTLStoreActionDontCare;
    stencilAttachment.loadAction = MTLLoadActionClear;
    stencilAttachment.clearStencil = 1;

    // making command encoder with render pass descriptor
    mCommandBuffer = [mCommandQueue commandBuffer];
    mCommandEncoder = [mCommandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    mRenderer->setCommandEncoder(mCommandEncoder);
  }

  void MetalFrameBuffer::end() {
    [mCommandEncoder endEncoding];
    [mCommandBuffer presentDrawable:mNextDrawable];
    [mCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _) {
      mSemaphore.notify();
    }];
    [mCommandBuffer commit];
  }

  void MetalFrameBuffer::makeDepthStencilTexture() {
    CGSize drawableSize = mRenderer->getMetalLayer().drawableSize;

    // recreate texture when depth texture size and drawable size is different.
    if ([mDepthStencilTexture width] != drawableSize.width || [mDepthStencilTexture height] != drawableSize.height) {
      mWidth = static_cast<real>(drawableSize.width);
      mHeight = static_cast<real>(drawableSize.height);
      MTLTextureDescriptor* textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float_Stencil8
                                                                                                   width:static_cast<NSUInteger>(drawableSize.width)
                                                                                                  height:static_cast<NSUInteger>(drawableSize.height)
                                                                                               mipmapped:NO];
      textureDescriptor.usage = MTLTextureUsageRenderTarget;
      textureDescriptor.storageMode = MTLStorageModePrivate;
      if (mDepthStencilTexture != nil) {
        [mDepthStencilTexture release];
      }
      mDepthStencilTexture = [mRenderer->getDevice() newTextureWithDescriptor:textureDescriptor];
      LOG_TRACE("Depth stencil texture recreated with size {0}, {1}", drawableSize.width, drawableSize.height);
    }
  }
}

#endif