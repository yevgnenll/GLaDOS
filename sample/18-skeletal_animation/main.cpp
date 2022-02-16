#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube(
            "test", {"grid.png", "grid2.png", "grid3.png", "grid4.png", "grid5.png", "grid6.png"}, PixelFormat::RGBA32
        );

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0.2, 1});

        // flair
        Flair = createGameObject("Flair");
        if (!AssimpLoader::getInstance().loadFromFile("Flair.fbx", this, Flair)) {
            return false;
        }
        Flair->transform()->setLocalScale(Vec3{0.05, 0.05, 0.05});
        animator = Flair->getComponent<Animator>();
        animator->getClipNames(animaitonClips);

        for (GameObject* gameObject : Flair->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        // strafe
        Strafe = createGameObject("Strafe");
        if (!AssimpLoader::getInstance().loadFromFile("Strafe.fbx", this, Strafe)) {
            return false;
        }
        Strafe->transform()->setLocalScale(Vec3{0.05, 0.05, 0.05});
        Strafe->transform()->setLocalPosition(Vec3{0.3, 0, 0});
        animator2 = Strafe->getComponent<Animator>();

        for (GameObject* gameObject : Strafe->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        GameObject* plane = createGameObject("plane");
        Mesh* planeMesh = MeshGenerator::generateTexturedCube();
        Material* planeMat = NEW_T(Material);
        colorShader = Platform::getRenderer().createShaderProgramFromFile("colorVertex", "colorFragment");
        if (colorShader == nullptr) {
            return false;
        }
        planeMat->setShaderProgram(colorShader);
        plane->addComponent<MeshRenderer>(planeMesh, planeMat);
        plane->transform()->setLocalScale(Vec3{10, 0.1, 10});
        plane->transform()->translate(Vec3{0, -0.1, 0});

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));
        Input::addAxis("MovementX", NEW_T(InputHandler(KeyCode::KEY_RIGHT, KeyCode::KEY_LEFT, 0.1)));
        Input::addAxis("MovementY", NEW_T(InputHandler(KeyCode::KEY_UP, KeyCode::KEY_DOWN, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        animator->play(animaitonClips[curClipIndex]);
        animator2->play("mixamo.com");

        if (Input::isKeyDown(KeyCode::KEY_RETURN)) {
            curClipIndex = (curClipIndex + 1) % animaitonClips.size();
        }

        // character movement
        Vec3 rightMove = Vec3::right;
        rightMove *= Input::getAxisRaw("MovementX") * moveSpeed * deltaTime;
        Flair->transform()->translate(rightMove);

        Vec3 upMove = Vec3::forward;
        upMove *= Input::getAxisRaw("MovementY") * moveSpeed * deltaTime;
        Flair->transform()->translate(upMove);

        if (Input::isKeyPress(KeyCode::KEY_O)) {
            animSpeed = Math::clamp(animSpeed - 1, 0.f, 60.f);
            animator->getCurrentState()->setTicksPerSecond(animSpeed);
        }

        if (Input::isKeyPress(KeyCode::KEY_P)) {
            animSpeed = Math::clamp(animSpeed + 1, 0.f, 60.f);
            animator->getCurrentState()->setTicksPerSecond(animSpeed);
        }

        // camera rotation
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

        if (Input::isKeyDown(KeyCode::KEY_TAB)) {
            rasterizerDesc.mFillMode = (rasterizerDesc.mFillMode == FillMode::Lines) ? FillMode::Fill : FillMode::Lines;
            for (ShaderProgram* shaderProgram : shaderPrograms) {
                shaderProgram->setRasterizerState(rasterizerDesc);
            }
        }
    }

  private:
    real scrollSensitivity = 0.1f;
    real dragSensitivity = 7;
    real moveSensitivity = 1;
    real moveSpeed = 0.2;
    real animSpeed = 30;
    GameObject* Flair = nullptr;
    GameObject* Strafe = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    RasterizerDescription rasterizerDesc{};
    Vector<ShaderProgram*> shaderPrograms;
    Animator* animator = nullptr, *animator2 = nullptr;
    Vector<std::string> animaitonClips;
    int curClipIndex = 0;
    ShaderProgram* colorShader{nullptr};
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