#include "RenderState.h"

namespace GLaDOS {
    DepthStencilState::DepthStencilState(const DepthStencilDescription& desc) : mDepthStencilDescription{desc} {
    }

    SamplerState::SamplerState(const SamplerDescription& desc) : mSamplerDescription{desc} {
    }

    RasterizerState::RasterizerState(const RasterizerDescription& desc) : mRasterizerDescription{desc} {
    }

    RenderPipelineState::RenderPipelineState(const RenderPipelineDescription& desc) : mRenderPipelineDescription{desc} {
    }
}  // namespace GLaDOS