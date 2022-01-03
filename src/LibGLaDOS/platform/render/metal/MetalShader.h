#ifndef GLADOS_METALSHADER_H
#define GLADOS_METALSHADER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <string>
#include "platform/render/Shader.h"
#include "MetalRenderer.h"

namespace GLaDOS {
    class MetalShader : public Shader {
      public:
        MetalShader(const std::string& sourceCode);
        ~MetalShader() override;

        bool createShader() override;
        id<MTLFunction> getFunction();

      private:
        id<MTLFunction> mFunction{nil};
    };
}

#endif

#endif  // GLADOS_METALSHADER_H
