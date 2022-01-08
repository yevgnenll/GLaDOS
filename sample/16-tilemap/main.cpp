#include "GLaDOS.h"

using namespace GLaDOS;

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "16-tilemap", "GLaDOS", false};
        if (!Platform::getInstance().initialize(params)) {
            return -1;
        }

        while (Platform::getInstance().isRunning()) {
            Platform::getInstance().update();
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}