#include "GLaDOS.h"

using namespace GLaDOS;

int main(int argc, char** argv) {
    {
        Logger* logger = LoggerRegistry::getInstance().makeAndGetLogger("00-basic");
        PlatformParams params{1024, 800, "01-setup", "GLaDOS", false};
        if (!Platform::getInstance().initialize(params)) {
            LOG_ERROR(logger, "Platform initialize failed!");
            return -1;
        }

        LOG_TRACE(logger, "test123123");
        LOG_DEBUG(logger, "test123123");
        LOG_INFO(logger, "test123123");
        LOG_WARN(logger, "test123123");
        LOG_ERROR(logger, "test123123");
        String str = TEXT("한국어 지원 합니다.");
        LOG_TRACE(logger, String::toUTF8(str).c_str());

        while (Platform::getInstance().isRunning()) {
            Platform::getInstance().update();
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}