#ifndef GLADOS_METALRENDERABLE_H
#define GLADOS_METALRENDERABLE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"
#include "platform/render/Renderable.h"

namespace GLaDOS {
    class Logger;
    class MetalTextureBase;
    class Uniform;
    class VertexFormatHolder;
    class MetalShaderProgram;
    class MetalRenderable : public Renderable {
      public:
        MetalRenderable() = default;
        ~MetalRenderable() override;

        void build() override;
        void bindParams() override;

        id<MTLRenderPipelineState> getPipelineState() const;
        id<MTLBuffer> getVertexBuffer() const;
        id<MTLBuffer> getIndexBuffer() const;

      private:
        MTLVertexDescriptor* makeVertexDescriptor(VertexFormatHolder* vertexFormatHolder, MetalShaderProgram* shaderProgram);
        MTLVertexAttribute* findVertexAttribute(VertexSemantic semantic, NSArray<MTLVertexAttribute*>* vertexAttributes);

        static void setTexture(id<MTLRenderCommandEncoder> commandEncoder, MetalTextureBase* texture, Uniform* uniform);
        static Logger* logger;

        MTLVertexDescriptor* mVertexDescriptor{nil};
        id<MTLRenderPipelineState> mPipelineState{nil};
    };
}  // namespace GLaDOS

#endif

#endif
