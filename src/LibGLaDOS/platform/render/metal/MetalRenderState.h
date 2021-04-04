#ifndef GLADOS_METALRENDERSTATE_H
#define GLADOS_METALRENDERSTATE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/RenderState.h"

namespace GLaDOS {
  class MetalDepthStencilState : public DepthStencilState {
  public:
    MetalDepthStencilState(const DepthStencilDescription& desc);
    ~MetalDepthStencilState() override;

    id<MTLDepthStencilState> getMetalDepthStencilState();

  private:
    static constexpr MTLCompareFunction mapComparisonFunctionFrom(ComparisonFunction func);
    static constexpr MTLStencilOperation mapStencilOperatorFrom(StencilOperator op);

    MTLDepthStencilDescriptor* mDepthStencilDescriptor{nil};
    id<MTLDepthStencilState> mDepthStencilState{nil};
  };

  class MetalSamplerState : public SamplerState {
  public:
    MetalSamplerState(const SamplerDescription& desc);
    ~MetalSamplerState() override;

    id<MTLSamplerState> getMetalSamplerState();

  private:
    static constexpr MTLSamplerMinMagFilter mapSamplerMinMagFilterFrom(FilterMode mode);
    static constexpr MTLSamplerMipFilter mapSamplerMipFilterFrom(FilterMode mode);
    static constexpr MTLSamplerAddressMode mapSamplerAddressModeFrom(WrapMode mode);

    MTLSamplerDescriptor* mSamplerDescriptor{nil};
    id<MTLSamplerState> mSamplerState{nil};
  };

  class MetalRasterizerState : public RasterizerState {
  public:
    MetalRasterizerState(const RasterizerDescription& desc);
    ~MetalRasterizerState() override = default;
  };
}  // namespace GLaDOS

#endif

#endif  //GLADOS_METALRENDERSTATE_H
