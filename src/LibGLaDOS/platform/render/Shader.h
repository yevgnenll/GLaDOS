#ifndef GLADOS_SHADER_H
#define GLADOS_SHADER_H

#include "resource/Resource.h"

namespace GLaDOS {
    class Logger;
    class Shader : public Resource {
      public:
        Shader(const std::string& sourceCode);
        ~Shader() override = default;

        virtual bool createShader() = 0;
        std::string getShaderSourceCode() const;
        bool isCompiled() const;
        std::string getShaderFullName() const;

      protected:
        static Logger* logger;
        std::string mShaderSourceCode;
        bool mIsCompiled{false};
    };
}

#endif  // GLADOS_SHADER_H
