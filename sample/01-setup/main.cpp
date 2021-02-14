#include "GLaDOS.h"

using namespace GLaDOS;

void update(real deltaTime) {

}

bool init() {
  PlatformParams params{800, 600, "Powered by GLaDOS", "Test", false};
  if (!Platform::getInstance()->initialize(params)) {
    LOG_ERROR("Platform initialize failed!");
    return false;
  }

  Platform::getInstance()->setClearColor(Color{1, 0, 0, 1});
  ShaderProgram* shaderProgram = Platform::getRenderer()->createShaderProgram("basicVertex.metal", "basicFragment.metal");
  if (shaderProgram == nullptr) {
    return false;
  }
  Material material;
  material.setShaderProgram(shaderProgram);

  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
    Input::getInstance()->update();
    Timer::getInstance()->update();

    update(Timer::deltaTime());

    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      Platform::getInstance()->quit();
    }

    if (Input::isKeyDown(KeyCode::KEY_A)) {
      Platform::getInstance()->fullScreen(true);
    }

    if (Input::isKeyDown(KeyCode::KEY_S)) {
      Platform::getInstance()->fullScreen(false);
    }
  }

  return true;
}

int main(int argc, char** argv) {
  init();
  dumpMemory();
  return 0;
}