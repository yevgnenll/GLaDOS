#include "Shader.h"
#include "utils/LoggerRegistry.h"
#include "RootDir.h"
#include "platform/OSTypes.h"

namespace GLaDOS {
    Logger* Shader::logger = LoggerRegistry::getInstance().makeAndGetLogger("Shader");
    Shader::Shader(const std::string& sourceCode) : Resource{ResourceType::Shader}, mShaderSourceCode{sourceCode} {
       setResourceDir(SHADER_DIR);
    }

    std::string Shader::getShaderSourceCode() const {
        return mShaderSourceCode;
    }

    bool Shader::isCompiled() const {
        return mIsCompiled;
    }

    std::string Shader::getShaderFullName() const {
        return mFileDirectory + mName + SHADER_SUFFIX;
    }
}