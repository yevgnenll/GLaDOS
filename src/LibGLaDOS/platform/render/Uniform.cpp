#include "Uniform.h"

namespace GLaDOS {
    bool Uniform::isUniformType() const {
        return mUniformType != UniformType::Unknown && mUniformType != UniformType::Texture && mUniformType != UniformType::Sampler;
    }

    bool Uniform::isTextureType() const {
        return mUniformType == UniformType::Texture;
    }

    std::string Uniform::toString() const {
        std::string outputString;

        outputString.append("ShaderType: ")
            .append(mShaderType.toString()).append(", ")
            .append("UniformType: ")
            .append(mUniformType.toString()).append(", ")
            .append("Name: ").append(mName).append(", ")
            .append("count: ").append(std::to_string(mCount)).append(", ")
            .append("offset: ").append(std::to_string(mOffset)).append(", ")
            .append("size: ").append(std::to_string(size()));

        return outputString;
    }
}  // namespace GLaDOS