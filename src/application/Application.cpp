#include "Application.h"

#include "RootDir.h"
#include "memory/Allocation.h"
#include "platform/Input.h"
#include "platform/Timer.h"
#include "utils/Utility.h"

namespace GameEngine {
  Application* Application::instance = nullptr;
  std::string Application::resourcePath = RESOURCE_DIR;
  std::string Application::shaderPath = SHADER_DIR;

  Application::Application() {
    Application::instance = this;
  }

  Application::~Application() {
    DELETE_T(mTimer, Timer);
    DELETE_T(mInput, Input);
    DELETE_T(mPlatform, Platform);
    DELETE_T(mDebug, Debug);
  }

  bool Application::initialize(const PlatformParams& params) {
    mDebug = NEW_T(Debug("GameEngine"));
    mPlatform = NEW_T(Platform);
    if (!mPlatform->initialize(params)) {
      LOG_ERROR("Platform initialize failed!");
      return false;
    }
    mInput = NEW_T(Input);
    mTimer = NEW_T(Timer);
    LOG_TRACE("Resource path: {0}", getResourcePath());
    LOG_TRACE("ShaderProgram path: {0}", getShaderPath());

    return true;
  }

  void Application::run() {
    while (mPlatform->isRunning()) {
      mPlatform->update();
      mInput->update();
      mTimer->update();

      update(Timer::deltaTime());
      render();

      if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
        mPlatform->quit();
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

  void Application::update(float deltaTime) {
  }
}  // namespace GameEngine