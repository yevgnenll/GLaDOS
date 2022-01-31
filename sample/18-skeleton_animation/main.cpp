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

        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("boneVertex", "boneFragment");
        shaderProgram->setRasterizerState(rasterizerDesc);

        shaderProgram2 = Platform::getRenderer().createShaderProgramFromFile("boneVertex", "boneFragment");
        shaderProgram2->setRasterizerState(rasterizerDesc);

        AssimpLoader loader;
        if (!loader.loadFromFile("xbot@Idle.fbx")) {
            return false;
        }

        Mesh* mesh1 = loader.getMesh()[0];
        Mesh* mesh2 = loader.getMesh()[1];

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        target = createGameObject("target");
        target->addComponent<MeshRenderer>(mesh1, material);
        transform1 = target->transform();
        transform1->setLocalScale(Vec3{0.01, 0.01, 0.01});
        transform1->setLocalPosition(Vec3{0, -1, 3});

        Material* material2 = NEW_T(Material);
        material2->setShaderProgram(shaderProgram2);

        target2 = createGameObject("target2", target);
        target2->addComponent<MeshRenderer>(mesh2, material2);
        transform2 = target2->transform();

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        transform1->rotate(Vec3{0, deltaTime * 50, 0});

        shaderProgram->setUniform("invModelView", Mat4<real>::inverse(transform1->localToWorldMatrix() * camera->worldToCameraMatrix()));
        shaderProgram->setUniform("viewPos", cameraTransform->localPosition());
        shaderProgram->setUniform("model", transform1->localToWorldMatrix());
        shaderProgram->setUniform("view", camera->worldToCameraMatrix());
        shaderProgram->setUniform("projection", camera->projectionMatrix());

        shaderProgram2->setUniform("invModelView", Mat4<real>::inverse(transform2->localToWorldMatrix() * camera->worldToCameraMatrix()));
        shaderProgram2->setUniform("viewPos", cameraTransform->localPosition());
        shaderProgram2->setUniform("model", transform2->localToWorldMatrix());
        shaderProgram2->setUniform("view", camera->worldToCameraMatrix());
        shaderProgram2->setUniform("projection", camera->projectionMatrix());

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

        if (Input::isKeyPress(KeyCode::KEY_RIGHT)) {
            transform1->translate(Vec3{0.02, 0, 0});
        } else if (Input::isKeyPress(KeyCode::KEY_LEFT)) {
            transform1->translate(Vec3{-0.02, 0, 0});
        }

        // camera rotation
        if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            cameraTransform->rotate(cameraTransform->right(), Deg{rotationX});
            cameraTransform->rotate(UVec3::up, Deg{-rotationY});
        }

        if (Input::isKeyDown(KeyCode::KEY_TAB)) {
            rasterizerDesc.mFillMode = (rasterizerDesc.mFillMode == FillMode::Lines) ? FillMode::Fill : FillMode::Lines;
            shaderProgram->setRasterizerState(rasterizerDesc);
            shaderProgram2->setRasterizerState(rasterizerDesc);
        }
    }

  private:
    real sensitivity = 5;
    GameObject* target = nullptr;
    GameObject* target2 = nullptr;
    ShaderProgram* shaderProgram = nullptr;
    ShaderProgram* shaderProgram2 = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    Transform* transform1 = nullptr;
    Transform* transform2 = nullptr;
    RasterizerDescription rasterizerDesc{};
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