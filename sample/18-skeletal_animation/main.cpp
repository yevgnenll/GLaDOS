#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        TextureCube* cubemap = Platform::getRenderer().createTextureCube(
            "cubeMap", {"grid.png", "grid2.png", "grid3.png", "grid4.png", "grid5.png", "grid6.png"}, PixelFormat::RGBA32
        );

        GameObject* cubemapObject = createGameObject("cubemap");
        CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
        cubemapRenderer->setTextureCube(cubemap);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0.3, 0.2, 1});

        if (!loadPrefab()) {
            return false;
        }

        GameObject* plane = createGameObject("plane");
        Mesh* planeMesh = MeshGenerator::generateTexturedCube();
        Material* planeMat = NEW_T(Material);
        colorShader = Platform::getRenderer().createShaderProgramFromFile("colorVertex", "colorFragment");
        if (colorShader == nullptr) {
            return false;
        }
        planeMat->setShaderProgram(colorShader);
        planeMat->setAlbedo(Color::grey);
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

    bool loadPrefab() {
        // flair
        flair = createGameObject("Flair");
        if (!Platform::getRenderer().createPrefabFromFile("Flair.fbx", flair)) {
            return false;
        }
        flair->transform()->setLocalScale(Vec3{0.05, 0.05, 0.05});
        animator = flair->getComponent<Animator>();

        for (GameObject* gameObject : flair->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        // strafe
        strafe = createGameObject("Strafe");
        if (!Platform::getRenderer().createPrefabFromFile("Strafe.fbx", strafe)) {
            return false;
        }
        strafe->transform()->setLocalScale(Vec3{0.05, 0.05, 0.05});
        strafe->transform()->setLocalPosition(Vec3{0.3, 0, 0});
        animator2 = strafe->getComponent<Animator>();

        for (GameObject* gameObject : strafe->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        // woman
        woman = createGameObject("woman");
        if (!Platform::getRenderer().createPrefabFromFile("Woman.gltf", woman)) {
            return false;
        }
        woman->transform()->setLocalScale(Vec3{0.03, 0.03, 0.03});
        woman->transform()->setLocalPosition(Vec3{0.6, 0, 0});
        animator3 = woman->getComponent<Animator>();
        animator3->getClipNames(animaitonClips);

        for (GameObject* gameObject : woman->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        // stormTrooper
        stormTrooper = createGameObject("stormTrooper");
        if (!Platform::getRenderer().createPrefabFromFile("dancing-stormtrooper/source/silly_dancing.fbx", stormTrooper)) {
            return false;
        }
        stormTrooper->transform()->setLocalScale(Vec3{0.37, 0.37, 0.37});
        stormTrooper->transform()->setLocalPosition(Vec3{-0.3, 0, 0});
        animator4 = stormTrooper->getComponent<Animator>();

        for (GameObject* gameObject : stormTrooper->getChildren()) {
            SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
            if (skinnedMeshRenderer != nullptr) {
                shaderPrograms.emplace_back(skinnedMeshRenderer->getRenderable()->getMaterial()->getShaderProgram());
            }
        }

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        animator->play("mixamo.com");
        animator2->play("mixamo.com");
        animator3->play(animaitonClips[curClipIndex]);
        animator4->play("mixamo.com");

        if (Input::isKeyDown(KeyCode::KEY_RETURN)) {
            curClipIndex = (curClipIndex + 1) % animaitonClips.size();
        }

        // character movement
        Vec3 rightMove = Vec3::right;
        rightMove *= Input::getAxisRaw("MovementX") * moveSpeed * deltaTime;
        flair->transform()->translate(rightMove);

        Vec3 upMove = Vec3::forward;
        upMove *= Input::getAxisRaw("MovementY") * moveSpeed * deltaTime;
        flair->transform()->translate(upMove);

        if (Input::isKeyPress(KeyCode::KEY_O)) {
            animSpeed = Math::clamp(animSpeed - 100, 0.f, 2000.f);
            animator3->getCurrentState()->setTicksPerSecond(animSpeed);
        }

        if (Input::isKeyPress(KeyCode::KEY_P)) {
            animSpeed = Math::clamp(animSpeed + 100, 0.f, 2000.f);
            animator3->getCurrentState()->setTicksPerSecond(animSpeed);
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
    real animSpeed = 1000;
    GameObject* flair = nullptr, *strafe = nullptr, *woman = nullptr, *stormTrooper;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
    RasterizerDescription rasterizerDesc{};
    Vector<ShaderProgram*> shaderPrograms;
    Animator* animator = nullptr, *animator2 = nullptr, *animator3 = nullptr, *animator4;
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