#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalGPUBuffer.h"
#include "MetalRenderable.h"
#include "MetalShaderProgram.h"
#include "MetalTexture2D.h"
#include "MetalTextureCube.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Uniform.h"

namespace GLaDOS {
    Logger* MetalRenderable::logger = LoggerRegistry::getInstance().makeAndGetLogger("MetalRenderable");
    MetalRenderable::~MetalRenderable() {
        [mPipelineState release];
    }

    void MetalRenderable::build() {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil || mMesh == nullptr || mMaterial == nullptr) {
            LOG_ERROR(logger, "Invalid renderable state");
            return;
        }

        MetalShaderProgram* shaderProgram = static_cast<MetalShaderProgram*>(mMaterial->getShaderProgram());  // INTEND: do not use dynamic_cast here
        if (shaderProgram == nullptr || !shaderProgram->isValid() || mVertexDescriptor != nil) {
            LOG_ERROR(logger, "Invalid shader program state");
            return;
        }

        mVertexDescriptor = shaderProgram->makeVertexDescriptor(mMesh->getVertexFormatHolder());
        MTLRenderPipelineDescriptor* pipelineDescriptor = shaderProgram->getPipelineDescriptor();
        if (pipelineDescriptor == nil || mPipelineState != nil) {
            LOG_ERROR(logger, "Invalid pipeline descriptor");
            return;
        }

        NSError* error;
        [pipelineDescriptor setVertexDescriptor:mVertexDescriptor];
        mPipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor options:MTLPipelineOptionArgumentInfo reflection:nil error:&error];
        if (mPipelineState == nil || error != nil) {
            LOG_ERROR(logger, "{0}", [[NSString stringWithFormat:@"%@", error] UTF8String]);
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

        id<MTLRenderCommandEncoder> commandEncoder = MetalRenderer::getInstance().getCommandEncoder();
        for (const auto& [key, uniform] : shaderProgram->getUniforms()) {
            if (!uniform->isTextureType()) {
                continue;
            }

            Texture* texture = mMaterial->getTextureFromIndex(uniform->mOffset);
            if (texture == nullptr) {
                LOG_ERROR(logger, "texture [offset({0})] in {1} should not be null", uniform->mOffset, uniform->mShaderType.toString());
                continue;
            }

            switch (texture->getDimension()) {
                case TextureDimension::Tex2D: {
                    MetalTexture2D* texture2D = static_cast<MetalTexture2D*>(mMaterial->getTextureFromIndex(uniform->mOffset));
                    if (texture2D == nullptr) {
                        continue;
                    }
                    MetalRenderable::setTexture(commandEncoder, texture2D, uniform);
                    break;
                }
                case TextureDimension::CubeMapTex: {
                    MetalTextureCube* textureCube = static_cast<MetalTextureCube*>(mMaterial->getTextureFromIndex(uniform->mOffset));
                    if (textureCube == nullptr) {
                        continue;
                    }
                    MetalRenderable::setTexture(commandEncoder, textureCube, uniform);
                    break;
                }
                default:
                    continue;
            }
        }

        shaderProgram->bindUniforms(this);
    }

    void MetalRenderable::setTexture(id<MTLRenderCommandEncoder> commandEncoder, MetalTextureBase* texture, Uniform* uniform) {
        if (uniform->mShaderType == ShaderType::VertexShader) {
            [commandEncoder setVertexTexture:texture->texture() atIndex:uniform->mOffset];
            [commandEncoder setVertexSamplerState:texture->metalSamplerState() atIndex:uniform->mOffset];
        } else if (uniform->mShaderType == ShaderType::FragmentShader) {
            [commandEncoder setFragmentTexture:texture->texture() atIndex:uniform->mOffset];
            [commandEncoder setFragmentSamplerState:texture->metalSamplerState() atIndex:uniform->mOffset];
        }
    }

    id<MTLRenderPipelineState> MetalRenderable::getPipelineState() const {
        return mPipelineState;
    }

    id<MTLBuffer> MetalRenderable::getVertexBuffer() const {
        auto* buffer = static_cast<MetalGPUBuffer*>(mMesh->getVertexBuffer());  // INTEND: do not use dynamic_cast here
        if (buffer == nullptr) {
            return nullptr;
        }
        return buffer->getMetalBuffer();
    }

    id<MTLBuffer> MetalRenderable::getIndexBuffer() const {
        auto* buffer = static_cast<MetalGPUBuffer*>(mMesh->getIndexBuffer());  // INTEND: do not use dynamic_cast here
        if (buffer == nullptr) {
            return nullptr;
        }
        return buffer->getMetalBuffer();
    }
}

#endif