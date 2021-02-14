#ifndef GLADOS_MATERIAL_H
#define GLADOS_MATERIAL_H

namespace GLaDOS {
  class ShaderProgram;
  class Material {
  public:
    Material() = default;
    ~Material() = default;

    ShaderProgram* getShaderProgram() const;
    void setShaderProgram(ShaderProgram* shaderProgram);

  private:
    ShaderProgram* mShaderProgram{nullptr};
  };
}  // namespace GLaDOS

#endif
