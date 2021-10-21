#ifndef GLADOS_METALTYPES_H
#define GLADOS_METALTYPES_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "MetalRenderer.h"

namespace GLaDOS {
    class Logger;
    class MetalTypes {
      public:
        MetalTypes() = delete;
        ~MetalTypes() = delete;

        static MTLVertexFormat vertexAttribTypeToMetal(VertexAttributeType type, bool normalized = false);
        static std::size_t metalVertexFormatToSize(MTLVertexFormat vertexFormat);
        static UniformType metalDataTypeToUniformType(MTLDataType dataType);
        static MTLTriangleFillMode fillModeToMetal(FillMode mode);
        static MTLWinding windingModeToMetal(WindingMode mode);
        static MTLCullMode cullModeToMetal(CullMode mode);
        static MTLVertexFormat metalDataTypeToVertexFormat(MTLDataType dataType, bool normalized = false);
        static MTLPrimitiveType primitiveTopologyToMetal(PrimitiveTopology primitiveTopology);
        static MTLIndexType sizeToMetalIndexType(std::size_t size);
        static MTLCompareFunction comparisonFunctionToMetal(ComparisonFunction func);
        static MTLStencilOperation stencilOperatorToMetal(StencilOperator op);
        static MTLSamplerMinMagFilter filterModeToMetalMinMagFilter(FilterMode mode);
        static MTLSamplerMipFilter filterModeToMetalMipFilter(FilterMode mode);
        static MTLSamplerAddressMode wrapModeToMetalAddressMode(WrapMode mode);
        static MTLPixelFormat pixelFormatToMetal(PixelFormat format);
        static MTLTextureUsage textureUsageToMetal(TextureUsage usage);

      private:
        static Logger* logger;
    };
}

#endif

#endif  // GLADOS_METALTYPES_H
