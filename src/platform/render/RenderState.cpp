#include "RenderState.h"

namespace GLaDOS {
  DepthStencilState::DepthStencilState(const DepthStencilDescription& desc) : mDescription{desc} {
  }

  SamplerState::SamplerState(const SamplerDescription& desc) : mSamplerDescription{desc} {
  }
}  // namespace GLaDOS