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

        DepthStencilDescription mDepthStencilDescription;
    };

    struct SamplerDescription {
        FilterMode mMinFilter{FilterMode::Bilinear};
        FilterMode mMagFilter{FilterMode::Bilinear};
        FilterMode mMipFilter{FilterMode::Bilinear};
        WrapMode mSWrap{WrapMode::ClampEdge};
        WrapMode mTWrap{WrapMode::ClampEdge};
        WrapMode mRWrap{WrapMode::ClampEdge};
        uint8_t mMaxAnisotropyLevel{1};
    };

    class SamplerState : public RenderState {
      public:
        SamplerState(const SamplerDescription& desc);
        ~SamplerState() override = default;

        SamplerDescription mSamplerDescription;
    };

    struct RasterizerDescription {
        CullMode mCullMode{CullMode::Back};
        WindingMode mWindingMode{WindingMode::CounterClockWise};
        FillMode mFillMode{FillMode::Fill};
        real mDepthBias{0};  // default no bias
        real mSlopeScaleDepthBias{0};  // default no slope scale bias
        real mDepthBiasClamp{0};  // default no depth bias clamping
        bool mIsScissorTestEnable{false};
        bool mIsMultisampleAntiAlias{false};
    };

    class RasterizerState : public RenderState {
      public:
        RasterizerState(const RasterizerDescription& desc);
        ~RasterizerState() override = default;

        RasterizerDescription mRasterizerDescription;
    };

    struct RenderPipelineDescription {
        PixelFormat colorPixelFormat{PixelFormat::BGRA32};
        PixelFormat depthPixelFormat{PixelFormat::Depth32Stencil8};
        PixelFormat stencilPixelFormat{PixelFormat::Depth32Stencil8};
    };

    class RenderPipelineState : public RenderState {
      public:
        RenderPipelineState(const RenderPipelineDescription& desc);
        ~RenderPipelineState() override = default;

        RenderPipelineDescription mRenderPipelineDescription;
    };
}  // namespace GLaDOS

#endif  //GLADOS_RENDERSTATE_H
