#include "MetalRenderState.h"

#ifdef PLATFORM_MACOS

#include "MetalTypes.h"

namespace GLaDOS {
    Logger* MetalDepthStencilState::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalDepthStencilState");
    Logger* MetalSamplerState::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalSamplerState");

    MetalDepthStencilState::MetalDepthStencilState(const DepthStencilDescription& desc) : DepthStencilState{desc} {
        mDepthStencilDescriptor = [MTLDepthStencilDescriptor new];
        mDepthStencilDescriptor.depthWriteEnabled = static_cast<BOOL>(desc.mIsDepthWriteEnable);
        mDepthStencilDescriptor.depthCompareFunction = MetalTypes::comparisonFunctionToMetal(desc.mDepthFunction);
        // TODO
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

    MetalSamplerState::MetalSamplerState(const SamplerDescription& desc) : SamplerState{desc} {
        mSamplerDescriptor = [MTLSamplerDescriptor new];
        mSamplerDescriptor.minFilter = MetalTypes::filterModeToMetalMinMagFilter(desc.mMinFilter);
        mSamplerDescriptor.magFilter = MetalTypes::filterModeToMetalMinMagFilter(desc.mMagFilter);
        mSamplerDescriptor.mipFilter = MetalTypes::filterModeToMetalMipFilter(desc.mMipFilter);
        mSamplerDescriptor.sAddressMode = MetalTypes::wrapModeToMetalAddressMode(desc.mSWrap);
        mSamplerDescriptor.tAddressMode = MetalTypes::wrapModeToMetalAddressMode(desc.mTWrap);
        mSamplerDescriptor.rAddressMode = MetalTypes::wrapModeToMetalAddressMode(desc.mRWrap);
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
}

#endif