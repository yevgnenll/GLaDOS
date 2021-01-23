#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include "utils/Singleton.hpp"
#include "platform/Platform.h"

namespace GameEngine {
  class Application : public Singleton<Application> {
  public:
    Application() = default;
    ~Application() override = default;

    bool initialize(const PlatformParams& params);
    void run();
    static std::string getResourcePath();
    static std::string getShaderPath();

  private:
    void render();
    void update(float deltaTime);

    static std::string resourcePath;
    static std::string shaderPath;
  };
}  // namespace GameEngine

#endif
