#include "ShaderProgram.h"

#include "math/Color.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "platform/render/Uniform.h"
#include "RootDir.h"

namespace GLaDOS {
  ShaderProgram::ShaderProgram() : Resource{ResourceType::ShaderProgram} {
    setResourceDir(SHADER_DIR);
  }

  ShaderProgram::~ShaderProgram() {
    deallocValueInMap(mUniforms);
  }

  void ShaderProgram::setUniform(const std::string& name, int value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, unsigned int value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, float value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec2& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value.x << value.y;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec3& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value.x << value.y << value.z;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec4& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value.x << value.y << value.z << value.w;
  }

  void ShaderProgram::setUniform(const std::string& name, const Color& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << value.r << value.g << value.b << value.a;
  }

  void ShaderProgram::setUniform(const std::string& name, const float* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec2* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i].x << values[i].y;
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec3* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i].x << values[i].y << values[i].z;
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec4* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i].x << values[i].y << values[i].z << values[i].w;
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Color* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i].r << values[i].g << values[i].b << values[i].a;
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Mat4<real>& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    for (std::size_t i = 0; i < value.size(); ++i) {
      (*it->second) << *(value.pointer() + i);
    }
  }

  void ShaderProgram::setUniform(const std::string& name, bool value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("uniform {0} not exist", name);
      return;
    }

    (*it->second) << static_cast<int8_t>(value);
  }

  Uniform* ShaderProgram::getUniform(const std::string& name) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      return nullptr;
    }

    return it->second;
  }

  Map<std::string, Uniform*>& ShaderProgram::getUniforms() {
    return mUniforms;
  }

  std::size_t ShaderProgram::uniformSize() const {
    return mUniforms.size();
  }

  bool ShaderProgram::isValid() const {
    return mIsValid;
  }
}  // namespace GLaDOS