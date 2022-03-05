#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        GameObject* gui = createGameObject("gui");
        canvas = gui->addComponent<GUICanvas>();

        MessageBox* messageBox = NEW_T(MessageBox("Hello World!", "Click the \"close\" button."));
        messageBox->moveTo(
            (Platform::getInstance().getDrawableWidth() - messageBox->size().w) / 2.f,
            (Platform::getInstance().getDrawableHeight() - messageBox->size().w) / 2.f);
        canvas->addFrame(messageBox);

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }
    }

  private:
    GUICanvas* canvas = nullptr;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "25-gui", "GLaDOS", false};
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