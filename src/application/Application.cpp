#include "Application.h"

#include "RootDir.h"
#include "platform/Input.h"
#include "platform/Timer.h"
#include "utils/Utility.h"

namespace GLaDOS {
  std::string Application::resourcePath = RESOURCE_DIR;
  std::string Application::shaderPath = SHADER_DIR;

  bool Application::initialize(const PlatformParams& params) {
    if (!Platform::getInstance()->initialize(params)) {
      LOG_ERROR("Platform initialize failed!");
      return false;
    }
    LOG_TRACE("Resource path: {0}", getResourcePath());
    LOG_TRACE("ShaderProgram path: {0}", getShaderPath());

    return true;
  }

  void Application::run() {
    while (Platform::getInstance()->isRunning()) {
      Platform::getInstance()->update();
      Input::getInstance()->update();
      Timer::getInstance()->update();

      update(Timer::deltaTime());
      render();

      if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
        Platform::getInstance()->quit();
      }
    }
  }

  std::string Application::getResourcePath() {
    return resourcePath;
  }

  std::string Application::getShaderPath() {
    return shaderPath;
  }

  void Application::render() {
  }

  void Application::update(real deltaTime) {
  }
}  // namespace GLaDOS