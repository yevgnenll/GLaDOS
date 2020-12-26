#include "platform//Input.h"
#include "platform/Platform.h"
using namespace GameEngine;

int main() {
  PlatformParams params{1024, 800, "Powered by gameengine"};
  Platform platform;
  Input input;
  bool result = platform.initialize(params);
  if (!result) {
    printf("error");
    return -1;
  }
  while (platform.isRunning()) {
    platform.update();
    input.update();
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      platform.quit();
    }
  }

  return 0;
}
