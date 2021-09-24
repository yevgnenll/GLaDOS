#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 5});

        Texture2D* texture = Platform::getRenderer().createTexture2D("spritesheet.png", PixelFormat::RGBA32);
        if (!texture->loadTextureFromFile()) {
            return false;
        }

        SpriteSheet spriteSheet{texture, Sizei{16, 16}, 26};

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        // camera translation
        Vec3 right = cameraTransform->right();
        right *= Input::getAxis("Horizontal") * sensitivity * deltaTime;
        cameraTransform->translate(right);

        Vec3 up = cameraTransform->up();
        up *= Input::getAxis("Forward") * sensitivity * deltaTime;
        cameraTransform->translate(up);

        Vec3 forward = cameraTransform->forward();
        forward *= Input::getAxis("Vertical") * sensitivity * deltaTime;
        cameraTransform->translate(forward);

        // camera rotation
        if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Math::toRadians(rotationX));
            cameraTransform->rotate(UVec3::up, Math::toRadians(-rotationY));
        }
    }

  private:
    real sensitivity = 15;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
};

bool init() {
    PlatformParams params{1024, 800, "09-sprite", "GLaDOS", false};
    if (!Platform::getInstance().initialize(params)) {
        return false;
    }

    Platform::getInstance().setClearColor(Color{0, 0, 0, 1});
    return true;
}

int main(int argc, char** argv) {
    if (!init()) {
        return -1;
    }

    SceneManager::getInstance().setActiveScene(SceneManager::getInstance().createScene<MainScene>("testScene"));
    while (Platform::getInstance().isRunning()) {
        Platform::getInstance().update();
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}