#ifndef GLADOS_SHADERPROGRAM_H
#define GLADOS_SHADERPROGRAM_H

#include <string>

#include "math/Mat4.hpp"
#include "resource/Resource.h"
#include "utils/Utility.h"

namespace GLaDOS {
  class Uniform;
  class Color;
  class ShaderProgram : public Resource {
  public:
    ShaderProgram();
    ~ShaderProgram() override;

    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, unsigned int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const Vec2& value);
    void setUniform(const std::string& name, const Vec3& value);
    void setUniform(const std::string& name, const Vec4& value);
    void setUniform(const std::string& name, const Color& value);
    void setUniform(const std::string& name, const float* values, int count);
    void setUniform(const std::string& name, const Vec2* values, int count);
    void setUniform(const std::string& name, const Vec3* values, int count);
    void setUniform(const std::string& name, const Vec4* values, int count);
    void setUniform(const std::string& name, const Color* values, int count);
    void setUniform(const std::string& name, const Mat4<real>& value);
    void setUniform(const std::string& name, bool value);
    Uniform* getUniform(const std::string& name);
    Map<std::string, Uniform*>& getUniforms();
    std::size_t uniformSize() const;
    bool isValid() const;

  private:
    virtual bool createShaderProgram(const std::string& vertex, const std::string& fragment) = 0;

  protected:
    std::string mVertexShaderCode;
    std::string mFragmentShaderCode;
    Map<std::string, Uniform*> mUniforms;
    bool mIsValid{false};
  };
}  // namespace GLaDOS

#endif
