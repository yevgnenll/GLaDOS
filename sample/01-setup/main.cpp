#include "application/Application.h"

using namespace GameEngine;

int main(int argc, char** argv) {
  PlatformParams params{1024, 800, "Powered by gameengine"};
  if (!Application::getInstance()->initialize(params)) {
    return -1;
  }
  Application::getInstance()->run();
  return 0;
}
