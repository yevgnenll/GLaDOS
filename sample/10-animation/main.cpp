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
        cameraTransform->setLocalPosition({0, 0, 1});

        Texture2D* texture = Platform::getRenderer().createTexture2D("player.png", PixelFormat::RGBA32);
        SamplerDescription samplerDesc;
        samplerDesc.mMinFilter = FilterMode::Nearest;
        samplerDesc.mMagFilter = FilterMode::Nearest;
        samplerDesc.mMipFilter = FilterMode::Nearest;
        texture->setSamplerState(samplerDesc);

        std::string filename = std::string(RESOURCE_DIR) + "player.json";
        FileSystem file{filename, OpenMode::Read};
        if (!file.isOpen()) {
            std::cout << "file open failed: " << filename << std::endl;
            return false;
        }

        std::string jsonString;
        JsonNode rootJson;
        std::string errString;
        file.readAll(jsonString);
        if (!JsonParser::parse(rootJson, jsonString, errString)) {
            std::cout << "json parse error! cause: " << errString << std::endl;
            return false;
        }

        JsonNode::array animationNode = rootJson["animations"].getArray();
        for (const auto& element : animationNode) {
            std::cout << (*element)["name"].getString() << std::endl;
        }

        sprites = {
            // idle
            NEW_T(Sprite(texture, {12, 512, 39, 64}, {19, 0})),
            NEW_T(Sprite(texture, {76, 512, 39, 64}, {19, 0})),
            NEW_T(Sprite(texture, {140, 512, 39, 64}, {19, 0})),

            // walk
            NEW_T(Sprite(texture, {14, 448, 39, 64}, {19, 0})),
            NEW_T(Sprite(texture, {78, 448, 39, 63}, {19, 0})),
            NEW_T(Sprite(texture, {142, 448, 39, 64}, {19, 0})),
            NEW_T(Sprite(texture, {206, 448, 39, 63}, {19, 0})),

            // crouch attack
            NEW_T(Sprite(texture, {0, 384, 63, 36}, {32, 0})),
            NEW_T(Sprite(texture, {64, 384, 63, 36}, {32, 0})),

            // jump
            NEW_T(Sprite(texture, {142, 384, 39, 62}, {19, 0})),

            // alert
            NEW_T(Sprite(texture, {14, 320, 39, 61}, {19, 0})),
            NEW_T(Sprite(texture, {78, 320, 39, 62}, {19, 0})),
            NEW_T(Sprite(texture, {142, 320, 39, 63}, {19, 0})),

            // swing1
            NEW_T(Sprite(texture, {0, 256, 45, 58}, {28, 0})),
            NEW_T(Sprite(texture, {87, 256, 39, 63}, {21, 0})),
            NEW_T(Sprite(texture, {176, 256, 56, 57}, {12, 0})),

            // swing2
            NEW_T(Sprite(texture, {12, 192, 44, 64}, {17, 0})),
            NEW_T(Sprite(texture, {86, 192, 40, 62}, {10, 0})),
            NEW_T(Sprite(texture, {144, 192, 48, 59}, {15, 0})),

            // swing3
            NEW_T(Sprite(texture, {0, 128, 52, 59}, {32, 0})),
            NEW_T(Sprite(texture, {104, 128, 41, 60}, {18, 0})),
            NEW_T(Sprite(texture, {184, 128, 45, 58}, {8, 0})),

            // stab1
            NEW_T(Sprite(texture, {15, 64, 46, 61}, {41, 0})),
            NEW_T(Sprite(texture, {102, 64, 53, 58}, {10, 0})),

            // stab2
            NEW_T(Sprite(texture, {14, 0, 46, 64}, {34, 0})),
            NEW_T(Sprite(texture, {100, 0, 55, 57}, {7, 0})),
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

        if (Input::isKeyDown(KeyCode::KEY_T)) {
            camera->setOrthographic(!camera->isOrthographic());
        }

        if (Input::isKeyDown(KeyCode::KEY_P)) {
            spriteRenderer->setFlipX(!spriteRenderer->getFlipX());
        }

        if (Input::isKeyDown(KeyCode::KEY_O)) {
            spriteRenderer->setFlipY(!spriteRenderer->getFlipY());
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
    Vector<Sprite*> sprites;
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