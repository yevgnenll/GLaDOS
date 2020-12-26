#include "platform//Input.h"
#include "platform/Platform.h"
#include "memory/Allocation.h"

using namespace GameEngine;

void init() {
  PlatformParams params{1024, 800, "Powered by gameengine"};
  Platform platform;
  Input input;
  bool result = platform.initialize(params);
  if (!result) {
    printf("error");
    return;
  }
  while (platform.isRunning()) {
    platform.update();
    input.update();
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      platform.quit();
    }
  }
}

int main() {
  init();
  dumpMemory();
  return 0;
}
