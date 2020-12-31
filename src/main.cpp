#include "memory/Allocation.h"
#include "platform/Input.h"
#include "platform/Platform.h"
#include "utils/Debug.h"

using namespace GameEngine;

int main() {
  {
    Debug debug{"GameEngine"};
    PlatformParams params{1024, 800, "Powered by gameengine"};
    Platform platform;
    Input input;
    bool result = platform.initialize(params);
    if (!result) {
      printf("error");
      return -1;
    }
    while (platform.isRunning()) {
      if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
        platform.quit();
      }
      platform.update();
      input.update();
    }
  }
  dumpMemory();
  return 0;
}
