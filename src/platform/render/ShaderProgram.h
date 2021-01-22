#ifndef GAMEENGINE_SHADERPROGRAM_H
#define GAMEENGINE_SHADERPROGRAM_H

#include "resource/Resource.h"

namespace GameEngine {
  class ShaderProgram : public Resource {
  public:
    ShaderProgram() = default;
    ~ShaderProgram() override = default;

    virtual bool createShaderProgram() = 0;

  private:
  };
}  // namespace GameEngine

#endif
