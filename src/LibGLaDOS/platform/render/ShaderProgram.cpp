#include "ShaderProgram.h"

#include "RenderState.h"
#include "Renderer.h"
#include "RootDir.h"
#include "Uniform.h"
#include "math/Color.h"
#include "math/Mat4.hpp"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "platform/Platform.h"

namespace GLaDOS {
  ShaderProgram::ShaderProgram() : Resource{ResourceType::ShaderProgram} {
    setResourceDir(SHADER_DIR);
  }

  ShaderProgram::~ShaderProgram() {
    deallocValueInMap(mUniforms);
    DELETE_T(mDepthStencilState, DepthStencilState);
    DELETE_T(mRasterizerState, RasterizerState);
  }

  void ShaderProgram::setUniform(const std::string& name, int value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, unsigned int value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, float value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec2& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec3& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec4& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const Color& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, const float* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec2* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec3* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Vec4* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Color* values, int count) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    for (int i = 0; i < count; ++i) {
      (*it->second) << values[i];
    }
  }

  void ShaderProgram::setUniform(const std::string& name, const Mat4<real>& value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
      return;
    }

    (*it->second) << value;
  }

  void ShaderProgram::setUniform(const std::string& name, bool value) {
    auto it = mUniforms.find(name);
    if (it == mUniforms.end()) {
      LOG_WARN("default", "uniform {0} not exist", name);
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

  DepthStencilState* ShaderProgram::depthStencilState() {
    if (mDepthStencilState == nullptr) {
      DepthStencilDescription defaultDesc{};
      setDepthStencilState(defaultDesc);
    }
    return mDepthStencilState;
  }

  void ShaderProgram::setDepthStencilState(const DepthStencilDescription& desc) {
    if (mDepthStencilState != nullptr) {
      DELETE_T(mDepthStencilState, DepthStencilState);
    }
    mDepthStencilState = Platform::getRenderer().createDepthStencilState(desc);
  }

  RasterizerState* ShaderProgram::rasterizerState() {
    if (mRasterizerState == nullptr) {
      RasterizerDescription defaultDesc{};
      setRasterizerState(defaultDesc);
    }
    return mRasterizerState;
  }

  void ShaderProgram::setRasterizerState(const RasterizerDescription& desc) {
    if (mRasterizerState != nullptr) {
      DELETE_T(mRasterizerState, RasterizerState);
    }
    mRasterizerState = Platform::getRenderer().createRasterizerState(desc);
  }
}  // namespace GLaDOS