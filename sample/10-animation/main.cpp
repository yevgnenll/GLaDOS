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
        camera->setOrthographic(false);
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 1});

        Texture2D* texture = Platform::getRenderer().createTexture2D("player.png", PixelFormat::RGBA32);
        SamplerDescription samplerDesc;
        samplerDesc.mMinFilter = FilterMode::Nearest;
        samplerDesc.mMagFilter = FilterMode::Nearest;
        samplerDesc.mMipFilter = FilterMode::Nearest;
        texture->setSamplerState(samplerDesc);

        sprites = {
            NEW_T(Sprite(texture, {14, 448, 39, 64}, {19.5, 0})),
            NEW_T(Sprite(texture, {78, 448, 39, 63}, {19.5, 0})),
            NEW_T(Sprite(texture, {142, 448, 39, 64}, {19.5, 0})),
            NEW_T(Sprite(texture, {206, 448, 39, 63}, {19.5, 0}))
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
            if (spriteIndex >= sprites.size()) {
                spriteIndex = 0;
            }
            spriteRenderer->setSprite(sprites[spriteIndex]);
        }

        if (Input::isKeyDown(KeyCode::KEY_M)) {
            rasterizerDesc.mFillMode = FillMode::Lines;
            for (auto* sprite : sprites) {
                sprite->getRenderable()->getMaterial()->getShaderProgram()->setRasterizerState(rasterizerDesc);
            }
        }
        if (Input::isKeyDown(KeyCode::KEY_N)) {
            rasterizerDesc.mFillMode = FillMode::Fill;
            for (auto* sprite : sprites) {
                sprite->getRenderable()->getMaterial()->getShaderProgram()->setRasterizerState(rasterizerDesc);
            }
        }

        // camera translation
        Vec3 right = cameraTransform->right();
        right *= Input::getAxis("Horizontal") * moveSensitivity * deltaTime;
        cameraTransform->translate(right);

        Vec3 up = cameraTransform->up();
        up *= Input::getAxis("Forward") * moveSensitivity * deltaTime;
        cameraTransform->translate(up);

        Vec3 forward = cameraTransform->forward();
        forward *= Input::getAxis("Vertical") * moveSensitivity * deltaTime;
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
    real moveSensitivity = 120;
    real sensitivity = 15;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    GameObject* player = nullptr;
    Array<Sprite*, 4> sprites;
    SpriteRenderer* spriteRenderer = nullptr;
    int spriteIndex = 0;
    RasterizerDescription rasterizerDesc{};
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