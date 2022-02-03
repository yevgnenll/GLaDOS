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

        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("skinningVertex", "skinningFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        shaderProgram->setRasterizerState(rasterizerDesc);

        shaderProgram2 = Platform::getRenderer().createShaderProgramFromFile("skinningVertex", "skinningFragment");
        if (shaderProgram2 == nullptr) {
            return false;
        }
        shaderProgram2->setRasterizerState(rasterizerDesc);

        AssimpLoader loader;
        if (!loader.loadFromFile("xbot@Idle.fbx")) {
            return false;
        }

        parent = createGameObject("parent");
        parent->transform()->setLocalScale(Vec3{0.01, 0.01, 0.01});
        parent->transform()->setLocalPosition(Vec3{0, -1, 3});

        Mesh* mesh1 = loader.getMesh()[0];
        Mesh* mesh2 = loader.getMesh()[1];

        GameObject* rootBone = createBoneObject(*loader.getBone(), parent);

        Material* material1 = NEW_T(Material);
        material1->setShaderProgram(shaderProgram);

        GameObject* target = createGameObject("target", parent);
        target->addComponent<SkinnedMeshRenderer>(mesh1, material1, rootBone);

        Material* material2 = NEW_T(Material);
        material2->setShaderProgram(shaderProgram2);

        GameObject* target2 = createGameObject("target2", parent);
        target2->addComponent<SkinnedMeshRenderer>(mesh2, material2, rootBone);

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));
        Input::addAxis("MovementX", NEW_T(InputHandler(KeyCode::KEY_RIGHT, KeyCode::KEY_LEFT, 0.1)));
        Input::addAxis("MovementY", NEW_T(InputHandler(KeyCode::KEY_UP, KeyCode::KEY_DOWN, 0.1)));

        return true;
    }

    GameObject* createBoneObject(Bone bone, GameObject* parentGameObject) {
        GameObject* gameObject = createGameObject(bone.name, parentGameObject);
        gameObject->transform()->fromMat4(bone.boneTransformation);
        for (Bone& child : bone.children) {
            createBoneObject(child, gameObject);
        }
        return gameObject;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        parent->transform()->rotate(Vec3{0, deltaTime * 50, 0});

        // character movement
        Vec3 rightMove = Vec3::right;
        rightMove *= Input::getAxisRaw("MovementX") * sensitivity * deltaTime;
        parent->transform()->translate(rightMove);

        Vec3 upMove = Vec3::up;
        upMove *= Input::getAxisRaw("MovementY") * sensitivity * deltaTime;
        parent->transform()->translate(upMove);

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

        if (Input::isKeyDown(KeyCode::KEY_TAB)) {
            rasterizerDesc.mFillMode = (rasterizerDesc.mFillMode == FillMode::Lines) ? FillMode::Fill : FillMode::Lines;
            shaderProgram->setRasterizerState(rasterizerDesc);
            shaderProgram2->setRasterizerState(rasterizerDesc);
        }
    }

  private:
    real sensitivity = 5;
    GameObject* parent = nullptr;
    ShaderProgram* shaderProgram = nullptr;
    ShaderProgram* shaderProgram2 = nullptr;
    Camera* camera = nullptr;
    Transform* cameraTransform = nullptr;
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