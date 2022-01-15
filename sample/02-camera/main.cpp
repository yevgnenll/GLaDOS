#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        // clang-format off
        float quad[] = {
            0.5, -0.5, 0.0,   1.0, 0.0, 0.0, 1.0,
            -0.5, -0.5, 0.0,  0.0, 1.0, 0.0, 1.0,
            -0.5,  0.5, 0.0,  0.0, 0.0, 1.0, 1.0,

            0.5,  0.5, 0.0,   1.0, 1.0, 0.0, 1.0,
            0.5, -0.5, 0.0,   1.0, 0.0, 0.0, 1.0,
            -0.5,  0.5, 0.0,  0.0, 0.0, 1.0, 1.0,
        };
        // clang-format on
        VertexBuffer* vertexData = NEW_T(VertexBuffer(VertexFormatDescriptor().position().color(), 6));
        vertexData->setBufferData(quad);
        Mesh* mesh = Platform::getRenderer().createMesh(vertexData, nullptr);
        if (mesh == nullptr) {
            return false;
        }
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("basicVertex", "basicFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        RasterizerDescription desc;
        desc.mCullMode = CullMode::None;
        shaderProgram->setRasterizerState(desc);

        rectObject = createGameObject("rectObject");
        rectObject->addComponent<MeshRenderer>(mesh, material);
        instantiate(rectObject, {3, 0, 0});

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 2});

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        rectObject->transform()->rotate(Vec3{0, 0, deltaTime * 50});

        shaderProgram->setUniform("brightness", Math::lerp(0.f, 1.f, Math::pingPong(Timer::getInstance().elapsedTime() * 0.5f, 1.f)));
        shaderProgram->setUniform("model", rectObject->transform()->localToWorldMatrix());
        shaderProgram->setUniform("view", camera->worldToCameraMatrix());
        shaderProgram->setUniform("projection", camera->projectionMatrix());

        // camera translation
        Vec3 right = cameraTransform->right();
        right *= Input::getAxisRaw("Horizontal") * sensitivity * deltaTime;
        cameraTransform->translate(right);

        Vec3 up = cameraTransform->up();
        up *= Input::getAxisRaw("Forward") * sensitivity * deltaTime;
        cameraTransform->translate(up);

        Vec3 forward = cameraTransform->forward();
        forward *= Input::getAxisRaw("Vertical") * sensitivity * deltaTime;
        cameraTransform->translate(forward);

        // camera rotation
        if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Math::toRadians(Deg{rotationX}));
            cameraTransform->rotate(UVec3::up, Math::toRadians(Deg{-rotationY}));
        }
    }

  private:
    real sensitivity = 5;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    GameObject* rectObject = nullptr;
};

bool init() {
    PlatformParams params{1024, 800, "02-camera", "GLaDOS", false};
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
    static real limitFPS = 1.0 / 60.0;
    real deltaTime = 0;

    while (Platform::getInstance().isRunning()) {
        Platform::getInstance().update();
        deltaTime += Timer::getInstance().deltaTime() / limitFPS;
        while (deltaTime >= 1.0) {
            // fixedUpdate();
            deltaTime--;
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}