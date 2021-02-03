#ifndef GLADOS_UNIFORM_H
#define GLADOS_UNIFORM_H

#include <string>

#include "memory/StreamBuffer.h"

namespace GLaDOS {
  class Uniform : public StreamBuffer {
  public:
    Uniform() = default;
    ~Uniform() override = default;

    bool isNumericUniformType() const;

    ShaderType mShaderType{ShaderType::Unknown};
    UniformType mUniformType{UniformType::Unknown};
    std::string mName{"unknown"};
    std::size_t mCount{0};
    std::size_t mSize{0};
    std::size_t mOffset{0};
  };
}  // namespace GLaDOS

#endif  //GLADOS_UNIFORM_H
