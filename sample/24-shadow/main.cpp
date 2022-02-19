#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        Camera* camera = getMainCamera();
        camera->gameObject()->transform()->setLocalPosition({0, 0, 1});

        Texture2D* shadowMap = Platform::getRenderer().createRenderTexture2D("shadowMap", 1024, 800, PixelFormat::Depth32);

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }
    }
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "24-shadow", "GLaDOS", false};
        if (!Platform::getInstance().initialize(params)) {
            return -1;
        }

        SceneManager::getInstance().setActiveScene(SceneManager::getInstance().createScene<MainScene>("testScene"));
        while (Platform::getInstance().isRunning()) {
            Platform::getInstance().update();
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}