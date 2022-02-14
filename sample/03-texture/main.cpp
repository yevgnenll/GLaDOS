#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        Mesh* mesh = MeshGenerator::generateTexturedCube();
        if (mesh == nullptr) {
            return false;
        }
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("textureVertex", "textureFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        DepthStencilDescription depthStencilDesc{};
        shaderProgram->setDepthStencilState(depthStencilDesc);

        Texture2D* cubeTexture = Platform::getRenderer().createTexture2D("cube.png", PixelFormat::RGBA32);

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        material->setTexture0(cubeTexture);

        GameObject* cube = createGameObject("cube");
        planeTransform = cube->transform();
        cube->addComponent<MeshRenderer>(mesh, material);

        camera = getMainCamera();
        camera->gameObject()->transform()->setLocalPosition({0, 0, 2});

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        if (Input::isMousePress(MouseButton::MOUSE_LEFT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            planeTransform->rotate(planeTransform->right(), Deg{rotationX});
            planeTransform->rotate(planeTransform->up(), Deg{-rotationY});
        }
    }

  private:
    real sensitivity = 15;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* planeTransform = nullptr;
};

bool init() {
    PlatformParams params{1024, 800, "03-texture", "GLaDOS", false};
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
    while (Platform::getInstance().isRunning()) {
        Platform::getInstance().update();
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}