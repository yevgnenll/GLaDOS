#include "GLaDOS.h"

using namespace GLaDOS;

int main(int argc, char** argv) {
  LoggerRegistry::getInstance().registerNewLogger(NEW_T(Logger("00-basic")));
  PlatformParams params{1024, 800, "01-setup", "GLaDOS", false};
  if (!Platform::getInstance().initialize(params)) {
    LOG_ERROR("default", "Platform initialize failed!");
    return -1;
  }

  LOG_TRACE("00-basic", "test123123");
  LOG_DEBUG("00-basic", "test123123");
  LOG_INFO("00-basic", "test123123");
  LOG_WARN("00-basic", "test123123");
  LOG_ERROR("00-basic", "test123123");
  String str = TEXT("한국어 지원 합니다.");
  printf("Test: %s\n", String::toUTF8(str.toStringView()).c_str());

  while (Platform::getInstance().isRunning()) {
    Platform::getInstance().update();
  }

  return 0;
}