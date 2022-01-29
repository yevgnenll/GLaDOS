#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 5});

        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("normalVertex", "normalFragment");
        if (shaderProgram == nullptr) {
            return false;
        }

        target = createGameObject("target");
        SkinnedMeshRenderer* rend = target->addComponent<SkinnedMeshRenderer>();
        Animator* anim = target->addComponent<Animator>();

//        Mesh* mesh = Platform::getRenderer().createMesh()

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        shaderProgram->setUniform("model", target->transform()->localToWorldMatrix());
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
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});
        }
    }

  private:
    real sensitivity = 5;
    GameObject* target = nullptr;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "18-skeleton_animation", "GLaDOS", false};
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