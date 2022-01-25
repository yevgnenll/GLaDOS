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
        String str = GTEXT("한국어 지원 합니다.");
        LOG_TRACE(logger, String::toUTF8(str).c_str());
        Quat q = {1, 2.223f, 3.444f, 0.123234f};
        LOG_INFO(logger, "{0}", q);
        Vec4 v1 = {1.424f, 213.f, 2.23f, 123.f};
        Vec3 v2 = {1.424f, 213.f, 2.23f};
        Vec2 v3 = {1.424f, 213.f};
        LOG_INFO(logger, "{0}", v1);
        LOG_INFO(logger, "{0}", v2);
        LOG_INFO(logger, "{0}", v3);
//        Mat4<real> m1 = Mat4<real>::identity();
//        LOG_INFO(logger, "{0}", m1);

        while (Platform::getInstance().isRunning()) {
            Platform::getInstance().update();
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}