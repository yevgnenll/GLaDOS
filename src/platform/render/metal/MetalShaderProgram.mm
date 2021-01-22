#include "MetalShaderProgram.h"

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

namespace GameEngine {
  bool MetalShaderProgram::createShaderProgram() {
    return false;
  }
}

#endif