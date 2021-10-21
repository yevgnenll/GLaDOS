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

        static constexpr MTLVertexFormat vertexAttribTypeToVertexFormat(VertexAttributeType type, bool normalized = false);
        static constexpr std::size_t metalVertexFormatToSize(MTLVertexFormat vertexFormat);
        static constexpr UniformType metalDataTypeToUniformType(MTLDataType dataType);
        static constexpr MTLTriangleFillMode fillModeToMetalFillMode(FillMode mode);
        static constexpr MTLWinding windingModeToMetalWinding(WindingMode mode);
        static constexpr MTLCullMode cullModeToMetalCullMode(CullMode mode);
        static constexpr MTLVertexFormat metalDataTypeToVertexFormat(MTLDataType dataType, bool normalized = false);
        static constexpr MTLPrimitiveType primitiveToMetalPrimitive(PrimitiveTopology primitiveTopology);
        static constexpr MTLIndexType sizeToMetalIndexType(std::size_t size);
        static constexpr MTLCompareFunction comparisonFunctionToMetal(ComparisonFunction func);
        static constexpr MTLStencilOperation stencilOperatorToMetal(StencilOperator op);
        static constexpr MTLSamplerMinMagFilter filterModeToMetalMinMagFilter(FilterMode mode);
        static constexpr MTLSamplerMipFilter filterModeToMetalMipFilter(FilterMode mode);
        static constexpr MTLSamplerAddressMode wrapModeToMetalAddressMode(WrapMode mode);
        static constexpr MTLPixelFormat pixelFormatToMetal(PixelFormat format);
        static constexpr MTLTextureUsage textureUsageToMetal(TextureUsage usage);

      private:
        static Logger* logger;
    };
}

#endif

#endif  // GLADOS_METALTYPES_H
