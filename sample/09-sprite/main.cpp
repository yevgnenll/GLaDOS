#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube("test", {"grid.png", "grid.png", "grid.png", "grid.png", "grid.png", "grid.png"}, PixelFormat::RGBA32);

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        camera->setOrthographic(true);
        camera->setUnitSize(5);
        camera->setViewportRect({0.25, 0.25, 0.5, 0.5});
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 1});

        Texture2D* texture = Platform::getRenderer().createTexture2D("spritesheet.png", PixelFormat::RGBA32);

        Sprite* sprite = NEW_T(Sprite(texture, {154, 9}));
        SamplerDescription samplerDesc;
        samplerDesc.mMinFilter = FilterMode::Nearest;
        samplerDesc.mMagFilter = FilterMode::Nearest;
        samplerDesc.mMipFilter = FilterMode::Nearest;
        texture->setSamplerState(samplerDesc);

        player = createGameObject("player");
        spriteRenderer = player->addComponent<SpriteRenderer>(sprite);

        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        player->transform()->rotate(Vec3{0, 0, deltaTime * 50});

        // camera translation
        Vec3 right = cameraTransform->right();
        right *= Input::getAxisRaw("Horizontal") * moveSensitivity * deltaTime;
        cameraTransform->translate(right);

        Vec3 up = cameraTransform->up();
        up *= Input::getAxisRaw("Vertical") * moveSensitivity * deltaTime;
        cameraTransform->translate(up);

        if (Input::isKeyDown(KeyCode::KEY_Z)) {
            spriteRenderer->setFlipX(!spriteRenderer->getFlipX());
        }

        if (Input::isKeyDown(KeyCode::KEY_X)) {
            spriteRenderer->setFlipY(!spriteRenderer->getFlipY());
        }

        spriteRenderer->setColor(Color::lerp(Color(1.f, 0.f, 0.f, 1.f), Color(0.f, 1.f, 1.f, 1.f), Math::pingPong(Timer::getInstance().elapsedTime(), 1.f)));
    }

  private:
    real moveSensitivity = 200;
    real sensitivity = 15;;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    GameObject* player = nullptr;
    SpriteRenderer* spriteRenderer = nullptr;
};

bool init() {
    PlatformParams params{
        .width = 1024,
        .height = 800,
        .titleName = "09-sprite",
        .name = "GLaDOS",
        .isFullscreen = false,
        .isShowCursor = true
    };
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