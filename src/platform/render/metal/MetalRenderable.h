#ifndef GLADOS_METALRENDERABLE_H
#define GLADOS_METALRENDERABLE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/Renderable.h"

namespace GLaDOS {
  class MetalRenderable : public Renderable {
  public:
    MetalRenderable() = default;
    ~MetalRenderable() override;

    void build() override;
    void bindParams() override;

    id<MTLRenderPipelineState> getPipelineState() const;
    id<MTLBuffer> getVertexBuffer() const;
    id<MTLBuffer> getIndexBuffer() const;

  private:
    MTLVertexDescriptor* mVertexDescriptor{nil};
    id<MTLRenderPipelineState> mPipelineState{nil};
  };
}  // namespace GLaDOS

#endif

#endif
