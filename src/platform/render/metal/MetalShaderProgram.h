#ifndef GAMEENGINE_METALSHADERPROGRAM_H
#define GAMEENGINE_METALSHADERPROGRAM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/Metal.h>

#include "platform/render/ShaderProgram.h"

namespace GameEngine {
  class MetalShaderProgram : public ShaderProgram {
  public:
    MetalShaderProgram() = default;
    ~MetalShaderProgram() override = default;

    bool createShaderProgram() override;

  private:
    id<MTLLibrary> mMetalVertexLibrary;
    id<MTLLibrary> mMetalFragmentLibrary;
    id<MTLFunction> mMetalVertexFunction;
    id<MTLFunction> mMetalFragmentFunction;
    MTLRenderPipelineDescriptor* mRenderPipelineDescriptor;
    MTLCompileOptions* compileOpts;
  };
}  // namespace GameEngine

#endif

#endif