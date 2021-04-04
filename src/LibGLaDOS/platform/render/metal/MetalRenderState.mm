#include "MetalRenderState.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  MetalDepthStencilState::MetalDepthStencilState(const DepthStencilDescription& desc) : DepthStencilState{desc} {
    mDepthStencilDescriptor = [MTLDepthStencilDescriptor new];
    mDepthStencilDescriptor.depthWriteEnabled = static_cast<BOOL>(desc.mIsDepthWriteEnable);
    mDepthStencilDescriptor.depthCompareFunction = MetalDepthStencilState::mapComparisonFunctionFrom(desc.mDepthFunction);
    // if (desc.mFrontStencilEnable) {
    // mDepthStencilDescriptor.frontFaceStencil = ;
    // }
    // if (desc.mBackStencilEnable) {
    // mDepthStencilDescriptor.backFaceStencil = ;
    // }

    id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
    if (device != nil) {
      mDepthStencilState = [device newDepthStencilStateWithDescriptor:mDepthStencilDescriptor];
    }
  }

  MetalDepthStencilState::~MetalDepthStencilState() {
    [mDepthStencilDescriptor release];
    [mDepthStencilState release];
  }

  id<MTLDepthStencilState> MetalDepthStencilState::getMetalDepthStencilState() {
    return mDepthStencilState;
  }

  constexpr MTLCompareFunction MetalDepthStencilState::mapComparisonFunctionFrom(ComparisonFunction func) {
    switch (func) {
      case ComparisonFunction::Never:
        return MTLCompareFunctionNever;
      case ComparisonFunction::Always:
        return MTLCompareFunctionAlways;
      case ComparisonFunction::Less:
        return MTLCompareFunctionLess;
      case ComparisonFunction::LessEqual:
        return MTLCompareFunctionLessEqual;
      case ComparisonFunction::Equal:
        return MTLCompareFunctionEqual;
      case ComparisonFunction::NotEqual:
        return MTLCompareFunctionNotEqual;
      case ComparisonFunction::Greater:
        return MTLCompareFunctionGreater;
      case ComparisonFunction::GreaterEqual:
        return MTLCompareFunctionGreaterEqual;
      default:
        LOG_WARN("default", "Unknown Depth comparison function! fallback to ComparisonFunction::Less");
        break;
    }

    return MTLCompareFunctionLess;
  }

  constexpr MTLStencilOperation MetalDepthStencilState::mapStencilOperatorFrom(StencilOperator op) {
    switch (op) {
      case StencilOperator::Keep:
        return MTLStencilOperationKeep;
      case StencilOperator::Zero:
        return MTLStencilOperationZero;
      case StencilOperator::Replace:
        return MTLStencilOperationReplace;
      case StencilOperator::Increase:
        return MTLStencilOperationIncrementClamp;
      case StencilOperator::IncreaseWrap:
        return MTLStencilOperationIncrementWrap;
      case StencilOperator::Decrease:
        return MTLStencilOperationDecrementClamp;
      case StencilOperator::DecreaseWrap:
        return MTLStencilOperationDecrementWrap;
      case StencilOperator::Invert:
        return MTLStencilOperationInvert;
      default:
        LOG_WARN("default", "Unknown Stencil operator! fallback to StencilOperator::Keep");
        break;
    }

    return MTLStencilOperationKeep;
  }

  MetalSamplerState::MetalSamplerState(const SamplerDescription& desc) : SamplerState{desc} {
    mSamplerDescriptor = [MTLSamplerDescriptor new];
    mSamplerDescriptor.minFilter = MetalSamplerState::mapSamplerMinMagFilterFrom(desc.mMinFilter);
    mSamplerDescriptor.magFilter = MetalSamplerState::mapSamplerMinMagFilterFrom(desc.mMagFilter);
    mSamplerDescriptor.mipFilter = MetalSamplerState::mapSamplerMipFilterFrom(desc.mMipFilter);
    mSamplerDescriptor.sAddressMode = MetalSamplerState::mapSamplerAddressModeFrom(desc.mSWrap);
    mSamplerDescriptor.tAddressMode = MetalSamplerState::mapSamplerAddressModeFrom(desc.mTWrap);
    mSamplerDescriptor.rAddressMode = MetalSamplerState::mapSamplerAddressModeFrom(desc.mRWrap);
    mSamplerDescriptor.maxAnisotropy = desc.mMaxAnisotropyLevel;

    id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
    if (device != nil) {
      mSamplerState = [device newSamplerStateWithDescriptor:mSamplerDescriptor];
    }
  }

  MetalSamplerState::~MetalSamplerState() {
    [mSamplerDescriptor release];
    [mSamplerState release];
  }

  id<MTLSamplerState> MetalSamplerState::getMetalSamplerState() {
    return mSamplerState;
  }

  MetalRasterizerState::MetalRasterizerState(const RasterizerDescription& desc) : RasterizerState{desc} {
  }

  constexpr MTLSamplerMinMagFilter MetalSamplerState::mapSamplerMinMagFilterFrom(FilterMode mode) {
    switch (mode) {
      case FilterMode::Nearest:
        return MTLSamplerMinMagFilterNearest;
      case FilterMode::Bilinear:
        return MTLSamplerMinMagFilterLinear;
      default:
        LOG_WARN("default", "Not recognized FilterMode in metal renderer fallback to Nearest");
    }

    return MTLSamplerMinMagFilterNearest;
  }

  constexpr MTLSamplerMipFilter MetalSamplerState::mapSamplerMipFilterFrom(FilterMode mode) {
    switch (mode) {
      case FilterMode::None:
        return MTLSamplerMipFilterNotMipmapped;
      case FilterMode::Nearest:
        return MTLSamplerMipFilterNearest;
      case FilterMode::Bilinear:
        return MTLSamplerMipFilterLinear;
      default:
        LOG_WARN("default", "Not recognized FilterMode in metal renderer fallback to Nearest");
        break;
    }

    return MTLSamplerMipFilterNotMipmapped;
  }

  constexpr MTLSamplerAddressMode MetalSamplerState::mapSamplerAddressModeFrom(WrapMode mode) {
    switch (mode) {
      case WrapMode::Clamp:
        return MTLSamplerAddressModeClampToZero;
      case WrapMode::ClampBorder:
        return MTLSamplerAddressModeClampToBorderColor;
      case WrapMode::ClampEdge:
        return MTLSamplerAddressModeClampToEdge;
      case WrapMode::Repeat:
        return MTLSamplerAddressModeRepeat;
      case WrapMode::MirroredRepeat:
        return MTLSamplerAddressModeMirrorRepeat;
      case WrapMode::MirroredClampEdge:
        return MTLSamplerAddressModeMirrorClampToEdge;
      default:
        LOG_WARN("default", "Not recognized WrapMode in metal renderer fallback to Clamp");
        break;
    }

    return MTLSamplerAddressModeClampToZero;
  }
}

#endif