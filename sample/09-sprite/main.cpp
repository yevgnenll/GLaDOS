#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube("test", PixelFormat::RGBA32);
        Vector<std::string> cubemapImages = {"grid.png", "grid.png", "grid.png", "grid.png", "grid.png", "grid.png"};
        if (!cubemap->loadTextureFromFile(cubemapImages)) {
            return false;
        }

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 21});

        Texture2D* texture = Platform::getRenderer().createTexture2D("player.png", PixelFormat::RGBA32);
        if (!texture->loadTextureFromFile()) {
            return false;
        }

        Sprite* sprite = NEW_T(Sprite(texture));
        SamplerDescription samplerDesc;
        samplerDesc.mMinFilter = FilterMode::Nearest;
        samplerDesc.mMagFilter = FilterMode::Nearest;
        samplerDesc.mMipFilter = FilterMode::Nearest;
        texture->setSamplerState(samplerDesc);

        player = createGameObject("player");
        spriteRenderer = player->addComponent<SpriteRenderer>(sprite);
        sprite->setPixelPerUnit(64);

        Texture2D* texture2 = Platform::getRenderer().createTexture2D("spritesheet.png", PixelFormat::RGBA32);
        if (!texture2->loadTextureFromFile()) {
            return false;
        }

        Sprite* sprite2 = NEW_T(Sprite(texture2));
        texture2->setSamplerState(samplerDesc);

        player2 = createGameObject("player2");
        player2->addComponent<SpriteRenderer>(sprite2);
        sprite->setPixelPerUnit(32);
        player2->transform()->setPosition({30, 0, 0});

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

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

        if (Input::isKeyDown(KeyCode::KEY_Z)) {
            spriteRenderer->setFlipX(!spriteRenderer->getFlipX());
        }

        if (Input::isKeyDown(KeyCode::KEY_X)) {
            spriteRenderer->setFlipY(!spriteRenderer->getFlipY());
        }
    }

  private:
    real sensitivity = 15;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    GameObject* player = nullptr;
    GameObject* player2 = nullptr;
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