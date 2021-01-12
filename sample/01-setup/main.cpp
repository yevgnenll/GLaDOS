#include "application/Application.h"
#include "memory/Allocation.h"

using namespace GameEngine;

int main(int argc, char** argv) {
  Application* application = NEW_T(Application);
  PlatformParams params{1024, 800, "Powered by gameengine"};
  if (!application->initialize(params)) {
    return -1;
  }
  application->run();
  DELETE_T(application, Application);
  dumpMemory();
  return 0;
}
