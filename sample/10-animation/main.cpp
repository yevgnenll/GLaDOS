#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube(
            "test", {"grid.png", "grid.png", "grid.png", "grid.png", "grid.png", "grid.png"}, PixelFormat::RGBA32
        );

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 21});

        Texture2D* texture = Platform::getRenderer().createTexture2D("player.png", PixelFormat::RGBA32);
        Texture2D* texture2 = Platform::getRenderer().createTexture2D("player.png", PixelFormat::RGBA32);
        SamplerDescription samplerDesc;
        samplerDesc.mMinFilter = FilterMode::Nearest;
        samplerDesc.mMagFilter = FilterMode::Nearest;
        samplerDesc.mMipFilter = FilterMode::Nearest;
        texture->setSamplerState(samplerDesc);
        texture2->setSamplerState(samplerDesc);

        sprites = {
            NEW_T(Sprite(texture, {12, 512, 39, 64}, {19.5, 32}, 64)),
            NEW_T(Sprite(texture2, {76, 512, 39, 64}, {19.5, 32}, 64)),
            NEW_T(Sprite(texture2, {140, 512, 39, 64}, {19.5, 32}, 64))
        };

        player = createGameObject("player");
        spriteRenderer = player->addComponent<SpriteRenderer>(sprites[spriteIndex]);

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        if (Input::isKeyDown(KeyCode::KEY_SPACE)) {
            spriteIndex++;
            if (spriteIndex >= 3) {
                spriteIndex = 0;
            }
            spriteRenderer->setSprite(sprites[spriteIndex]);
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
    GameObject* player = nullptr;
    Array<Sprite*, 3> sprites;
    SpriteRenderer* spriteRenderer = nullptr;
    int spriteIndex = 0;
};

bool init() {
    PlatformParams params{1024, 800, "10-animation", "GLaDOS", false};
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