#include "Uniform.h"

namespace GLaDOS {
  bool Uniform::isNumericUniformType() const {
    return mUniformType != UniformType::Unknown && mUniformType != UniformType::Texture && mUniformType != UniformType::Sampler;
  }
}  // namespace GLaDOS