#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube(
            "Cubemap", {"grid.png", "grid2.png", "grid3.png", "grid4.png", "grid5.png", "grid6.png"}, PixelFormat::RGBA32
        );

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 1});

        model = createGameObject("model");

        if (!Platform::getRenderer().createPrefabFromFile("Low Poly Pine/Low Poly Pine.obj", model)) {
            return false;
        }
        modelTransform = model->transform();
        modelTransform->setLocalScale(Vec3{0.03, 0.03, 0.03});

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        for (GameObject* gameObject : model->getChildren()) {
            MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
            if (meshRenderer != nullptr) {
                shaderPrograms.emplace_back(meshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        modelTransform->rotate(Vec3{0, 20.f * deltaTime, 0});

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
            real rotationX = mouseDelta.y * dragSensitivity * deltaTime;
            real rotationY = mouseDelta.x * dragSensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});
        }

        if (Input::isKeyDown(KeyCode::KEY_TAB)) {
            rasterizerDesc.mFillMode = (rasterizerDesc.mFillMode == FillMode::Lines) ? FillMode::Fill : FillMode::Lines;
            for (ShaderProgram* shaderProgram : shaderPrograms) {
                shaderProgram->setRasterizerState(rasterizerDesc);
            }
        }
    }

  private:
    real sensitivity = 1;
    real dragSensitivity = 5;
    GameObject* model = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr, *modelTransform = nullptr;
    RasterizerDescription rasterizerDesc{};
    Vector<ShaderProgram*> shaderPrograms;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "15-mesh", "GLaDOS", false};
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