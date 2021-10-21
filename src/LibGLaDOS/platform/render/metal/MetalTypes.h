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

      private:
        static Logger* logger;
    };
}

#endif

#endif  // GLADOS_METALTYPES_H
