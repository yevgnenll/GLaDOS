#ifndef GLADOS_APPLICATION_H
#define GLADOS_APPLICATION_H

#include "platform/Platform.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
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
    void update(real deltaTime);

    static std::string resourcePath;
    static std::string shaderPath;
  };
}  // namespace GLaDOS

#endif
