#include "Material.h"

namespace GLaDOS {
  ShaderProgram* Material::getShaderProgram() const {
    return mShaderProgram;
  }

  void Material::setShaderProgram(ShaderProgram* shaderProgram) {
    mShaderProgram = shaderProgram;
  }
}  // namespace GLaDOS