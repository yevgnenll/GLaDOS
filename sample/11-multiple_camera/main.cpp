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
        cameraTransform->setLocalPosition({0, 0, 5});

        Mesh* cube = MeshGenerator::generateCube();
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("normalVertex", "normalFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        cubeObject = createGameObject("rectObject");
        cubeObject->addComponent<MeshRenderer>(cube, material);

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
        right *= Input::getAxisRaw("Horizontal") * moveSensitivity * deltaTime;
        cameraTransform->translate(right);

        Vec3 up = cameraTransform->up();
        up *= Input::getAxisRaw("Forward") * moveSensitivity * deltaTime;
        cameraTransform->translate(up);

        Vec3 forward = cameraTransform->forward();
        forward *= Input::getAxisRaw("Vertical") * moveSensitivity * deltaTime;
        cameraTransform->translate(forward);

        // camera rotation
        if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});
        }

        if (Input::isMousePress(MouseButton::MOUSE_LEFT)) {
            Vec3 worldPoint = camera->screenToWorldPoint(Vec3::toVec2(Input::mousePosition()));
            std::cout << "world point: x=" << worldPoint.x << ", y=" << worldPoint.y << ", z=" << worldPoint.z << std::endl;
            cubeObject->transform()->setLocalPosition(worldPoint);
        }
    }

  private:
    real moveSensitivity = 10;
    real sensitivity = 15;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    GameObject* cubeObject = nullptr;
    ShaderProgram* shaderProgram = nullptr;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "11-multiple_camera", "GLaDOS", false};
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