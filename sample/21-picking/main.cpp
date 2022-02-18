#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 1});

        GameObject* sphere = createGameObject("Sphere");
        Mesh* mesh = MeshGenerator::generateIcoSphere();
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("normalVertex", "normalFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* mat = NEW_T(Material);
        mat->setShaderProgram(shaderProgram);
        sphere->addComponent<MeshRenderer>(mesh, mat);

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * dragSensitivity * deltaTime;
            real rotationY = mouseDelta.x * dragSensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});

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
        }

        if (Input::isMousePress(MouseButton::MOUSE_MIDDLE)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            Vec3 right = cameraTransform->right();
            Vec3 up = cameraTransform->up();
            right *= -1 * mouseDelta.x * scrollSensitivity * deltaTime;
            up *= -1 * mouseDelta.y * scrollSensitivity * deltaTime;
            cameraTransform->translate(right + up);
        }
    }

  private:
    real scrollSensitivity = 0.1f;
    real dragSensitivity = 7;
    real moveSensitivity = 1;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "21-picking", "GLaDOS", false};
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