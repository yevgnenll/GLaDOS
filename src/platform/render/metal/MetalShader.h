#ifndef GAMEENGINE_METALSHADER_H
#define GAMEENGINE_METALSHADER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/Shader.h"
#import <Metal/Metal.h>

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
}

#endif

#endif