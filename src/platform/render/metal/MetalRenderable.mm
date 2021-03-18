#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalBuffer.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Uniform.h"
#include "MetalTexture2D.h"

namespace GLaDOS {
  MetalRenderable::~MetalRenderable() {
    [mPipelineState release];
  }

  void MetalRenderable::build() {
    id<MTLDevice> device = MetalRenderer::getInstance()->getDevice();
    if (device == nil || mMesh == nullptr || mMaterial == nullptr) {
      LOG_ERROR("Invalid renderable state");
      return;
    }

    MetalShaderProgram* shaderProgram = static_cast<MetalShaderProgram*>(mMaterial->getShaderProgram());  // INTEND: do not use dynamic_cast here
    if (shaderProgram == nullptr && !shaderProgram->isValid() && mVertexDescriptor != nil) {
      LOG_ERROR("Invalid shader program state");
      return;
    }

    mVertexDescriptor = shaderProgram->makeVertexDescriptor(mMesh->getVertexFormats());
    MTLRenderPipelineDescriptor* pipelineDescriptor = shaderProgram->getPipelineDescriptor();
    if (pipelineDescriptor == nil && mPipelineState != nil) {
      LOG_ERROR("Invalid pipeline Descriptor");
      return;
    }

    NSError* error;
    [pipelineDescriptor setVertexDescriptor:mVertexDescriptor];
    mPipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor options:MTLPipelineOptionArgumentInfo reflection:nil error:&error];
    if (mPipelineState == nil || error != nil) {
      LOG_ERROR("{0}", [[NSString stringWithFormat:@"%@", error] UTF8String]);
    }
  }

  void MetalRenderable::bindParams() {
    if (mMaterial == nullptr) {
      return;
    }
    MetalShaderProgram* shaderProgram = static_cast<MetalShaderProgram*>(mMaterial->getShaderProgram());  // INTEND: do not use dynamic_cast here
    if (shaderProgram == nullptr) {
      return;
    }
    id<MTLRenderCommandEncoder> commandEncoder = MetalRenderer::getInstance()->getCommandEncoder();
    for (const auto& [key, uniform] : shaderProgram->getUniforms()) {
      if (!uniform->isTextureType()) {
        continue;
      }

      MetalTexture2D* textureN = static_cast<MetalTexture2D*>(mMaterial->getTextureFromIndex(uniform->mOffset));  // INTEND: do not use dynamic_cast here
      if (textureN == nullptr) {
        continue;
      }
      if (uniform->mShaderType == ShaderType::VertexShader) {
        [commandEncoder setVertexTexture:textureN->texture() atIndex:uniform->mOffset];
        [commandEncoder setVertexSamplerState:textureN->metalSamplerState() atIndex:uniform->mOffset];
      } else if (uniform->mShaderType == ShaderType::FragmentShader) {
        [commandEncoder setFragmentTexture:textureN->texture() atIndex:uniform->mOffset];
        [commandEncoder setFragmentSamplerState:textureN->metalSamplerState() atIndex:uniform->mOffset];
      }
    }

    shaderProgram->bindUniforms(this);
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