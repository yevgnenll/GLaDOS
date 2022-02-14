#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        target = createGameObject("target");
        Mesh* mesh = MeshGenerator::generateTexturedCube();
        if (mesh == nullptr) {
            return false;
        }
        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 5});

        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("normalVertex", "normalFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        target->addComponent<MeshRenderer>(mesh, material);
        target->transform()->scale({5, 0, 0}, Space::World);

        currentAngle = targetAngle0;

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
            changeCurrentAngle();
        }

        target->transform()->setRotation(Quat::slerp(target->transform()->rotation(), currentAngle, 0.2));

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
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});
        }
    }

    void changeCurrentAngle() {
        if (currentAngle == targetAngle0) {
            currentAngle = targetAngle90;
        } else {
            currentAngle = targetAngle0;
        }
    }

  private:
    Quat targetAngle90 = Quat::fromEuler({0, 0, 90});
    Quat targetAngle0 = Quat::fromEuler({0, 0, 0});
    Quat currentAngle;
    real sensitivity = 5;
    GameObject* target = nullptr;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "17-quat_lerp", "GLaDOS", false};
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