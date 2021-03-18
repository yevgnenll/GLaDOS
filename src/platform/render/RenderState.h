#ifndef GLADOS_RENDERSTATE_H
#define GLADOS_RENDERSTATE_H

#include "utils/Enumeration.h"

namespace GLaDOS {
  class RenderState {
  public:
    RenderState() = default;
    virtual ~RenderState() = default;
  };

  struct DepthStencilDescription {
    bool mIsDepthWriteEnable{true};
    ComparisonFunction mDepthFunction{ComparisonFunction::Less};
    bool mFrontStencilEnable{false};
    ComparisonFunction mFrontStencilFunction{ComparisonFunction::Never};
    StencilOperator mFrontStencilFailOp{StencilOperator::Keep};
    StencilOperator mFrontDepthFailOp{StencilOperator::Keep};
    StencilOperator mFrontStencilPassOp{StencilOperator::Keep};
    uint16_t mFrontStencilReadMask{0xFF};
    uint16_t mFrontStencilWriteMask{0xFF};
    uint32_t mFrontStencilRefValue{1};
    bool mBackStencilEnable{false};
    ComparisonFunction mBackStencilFunction{ComparisonFunction::Never};
    StencilOperator mBackStencilFailOp{StencilOperator::Keep};
    StencilOperator mBackDepthFailOp{StencilOperator::Keep};
    StencilOperator mBackStencilPassOp{StencilOperator::Keep};
    uint16_t mBackStencilReadMask{0xFF};
    uint16_t mBackStencilWriteMask{0xFF};
    uint32_t mBackStencilRefValue{1};
  };

  class DepthStencilState : public RenderState {
  public:
    DepthStencilState(const DepthStencilDescription& desc);
    ~DepthStencilState() override = default;

  protected:
    DepthStencilDescription mDescription;
  };

  struct SamplerDescription {
    FilterMode mMinFilter{FilterMode::Bilinear};
    FilterMode mMagFilter{FilterMode::Bilinear};
    FilterMode mMipFilter{FilterMode::Bilinear};
    WrapMode mSWrap{WrapMode::ClampEdge};
    WrapMode mTWrap{WrapMode::ClampEdge};
    WrapMode mRWrap{WrapMode::ClampEdge};
    uint8_t mMaxAnisotropyLevel{1};
    real mMipBias{0};
  };

  class SamplerState : public RenderState {
  public:
    SamplerState(const SamplerDescription& desc);
    ~SamplerState() override = default;

  protected:
    SamplerDescription mSamplerDescription;
  };
}  // namespace GLaDOS

#endif  //GLADOS_RENDERSTATE_H
