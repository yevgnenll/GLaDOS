#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include "platform/Platform.h"

namespace GameEngine {
  class Input;
  class Timer;
  class Debug;
  class Application {
  public:
    Application();
    ~Application();

    bool initialize(const PlatformParams& params);
    void run();
    static std::string getResourcePath();
    static std::string getShaderPath();

  private:
    static Application* instance;

    void render();
    void update(float deltaTime);

    Platform* mPlatform;
    Input* mInput;
    Timer* mTimer;
    Debug* mDebug;
    static std::string resourcePath;
    static std::string shaderPath;
  };
}  // namespace GameEngine

#endif
