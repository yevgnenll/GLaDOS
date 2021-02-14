#ifndef GLADOS_METALRENDERABLE_H
#define GLADOS_METALRENDERABLE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/Renderable.h"

namespace GLaDOS {
  class MetalShaderProgram;
  class MetalRenderable : public Renderable {
  public:
    MetalRenderable() = default;
    ~MetalRenderable() override = default;

    void build() override;
    void bindParams() override;

    id<MTLRenderPipelineState> getPipelineState() const;
    id<MTLBuffer> getVertexBuffer() const;
    id<MTLBuffer> getIndexBuffer() const;

  private:
    MTLVertexDescriptor* mVertexDescriptor{nullptr};
    id<MTLRenderPipelineState> mPipelineState{nil};
    MetalShaderProgram* mShaderProgram{nullptr};
  };
}  // namespace GLaDOS

#endif

#endif
