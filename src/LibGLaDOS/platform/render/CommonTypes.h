#ifndef GLADOS_COMMONTYPES_H
#define GLADOS_COMMONTYPES_H

#include "utils/Enumeration.h"

namespace GLaDOS {
    class Logger;
    class CommonTypes {
      public:
        CommonTypes() = delete;
        ~CommonTypes() = delete;

        static std::size_t vertexArrtibTypeToSize(VertexAttributeType type);
        static std::size_t uniformTypeToSize(UniformType uniformType);
        static const char* vertexSemanticToName(VertexSemantic semantic);

      private:
        static Logger* logger;
    };
}

#endif  // GLADOS_COMMONTYPES_H
