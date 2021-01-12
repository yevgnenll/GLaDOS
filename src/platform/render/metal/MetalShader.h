#ifndef GAMEENGINE_METALSHADER_H
#define GAMEENGINE_METALSHADER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Metal/Metal.h>

#include "platform/render/Shader.h"

namespace GameEngine {
  class MetalShader : public Shader {
  public:
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