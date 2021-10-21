#ifndef GLADOS_METALRENDERSTATE_H
#define GLADOS_METALRENDERSTATE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/RenderState.h"

namespace GLaDOS {
    class Logger;
    class MetalDepthStencilState : public DepthStencilState {
      public:
        MetalDepthStencilState(const DepthStencilDescription& desc);
        ~MetalDepthStencilState() override;

        id<MTLDepthStencilState> getMetalDepthStencilState();

      private:
        static Logger* logger;

        MTLDepthStencilDescriptor* mDepthStencilDescriptor{nil};
        id<MTLDepthStencilState> mDepthStencilState{nil};
    };

    class MetalSamplerState : public SamplerState {
      public:
        MetalSamplerState(const SamplerDescription& desc);
        ~MetalSamplerState() override;

        id<MTLSamplerState> getMetalSamplerState();

      private:
        static Logger* logger;

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
