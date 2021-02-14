#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalBuffer.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"

namespace GLaDOS {
  void MetalRenderable::build() {
    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nullptr || mMesh == nullptr || mMaterial == nullptr) {
      LOG_ERROR("Invalid renderable state");
      return;
    }

    mShaderProgram = dynamic_cast<MetalShaderProgram*>(mMaterial->getShaderProgram());
    if (mShaderProgram == nullptr && !mShaderProgram->isValid() && mVertexDescriptor != nullptr) {
      LOG_ERROR("Invalid shader program state");
      return;
    }

    mVertexDescriptor = mShaderProgram->makeVertexDescriptor(mMesh->getVertexFormats());
    MTLRenderPipelineDescriptor* pipelineDescriptor = mShaderProgram->getPipelineDescriptor();
    if (pipelineDescriptor == nullptr && mPipelineState != nullptr) {
      LOG_ERROR("Invalid pipeline Descriptor");
      return;
    }

    NSError* error = nullptr;
    [pipelineDescriptor setVertexDescriptor:mVertexDescriptor];
    mPipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor options:MTLPipelineOptionArgumentInfo reflection:nil error:&error];
    if (mPipelineState == nullptr || error != nullptr) {
      LOG_ERROR("{0}", [[NSString stringWithFormat:@"%@", error] UTF8String]);
    }
  }

  void MetalRenderable::bindParams() {
    //    for (const auto& [key, uniform] : mShaderProgram->getUniforms()) {
    //      // TODO
    //    }

    mShaderProgram->bindUniforms(this);
  }

  id<MTLRenderPipelineState> MetalRenderable::getPipelineState() const {
    return mPipelineState;
  }

  id<MTLBuffer> MetalRenderable::getVertexBuffer() const {
    auto* buffer = static_cast<MetalBuffer*>(mMesh->getVertexBuffer());  // INTEND: do not use dynamic_cast here
    if (buffer == nullptr) {
      return nullptr;
    }
    return buffer->getMetalBuffer();
  }

  id<MTLBuffer> MetalRenderable::getIndexBuffer() const {
    auto* buffer = static_cast<MetalBuffer*>(mMesh->getIndexBuffer());  // INTEND: do not use dynamic_cast here
    if (buffer == nullptr) {
      return nullptr;
    }
    return buffer->getMetalBuffer();
  }
}

#endif