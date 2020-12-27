#include "memory/Allocation.h"
#include "platform//Input.h"
#include "platform/Platform.h"

using namespace GameEngine;

int main() {
  PlatformParams params{1024, 800, "Powered by gameengine"};
  Platform* platform = NEW_T(Platform);
  Input input;
  bool result = platform->initialize(params);
  if (!result) {
    printf("error");
    return -1;
  }
  while (platform->isRunning()) {
    platform->update();
    input.update();
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      platform->quit();
    }
  }
  DELETE_T(platform, Platform);
  dumpMemory();
  return 0;
}
