#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        GameObject* cubemapObject = createGameObject("cubemap");
        TextureCube* cubemap = Platform::getRenderer().createTextureCube("CubeMap", {"px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"}, PixelFormat::RGBA32);
        if (cubemap == nullptr) {
            return false;
        }
        cubemapObject->addComponent<CubemapRenderer>()->setTextureCube(cubemap);

        Mesh* mesh = MeshGenerator::generateTorusKnot(3, 8, 256, 32, 0.2);
        if (mesh == nullptr) {
            return false;
        }
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("environmentVertex", "environmentFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        DepthStencilDescription depthStencilDesc{};
        shaderProgram->setDepthStencilState(depthStencilDesc);
        shaderProgram->setRasterizerState(rasterizerDesc);

        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        material->setTexture0(cubemap);

        knot = createGameObject("knot");
        knotTransform = knot->transform();
        knot->addComponent<MeshRenderer>(mesh, material);

        camera = getMainCamera();
        cameraTransform = camera->gameObject()->transform();
        cameraTransform->setLocalPosition({0, 0, 5});

        Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        knotTransform->rotate(Vec3{0, deltaTime * 50, 0});

        shaderProgram->setUniform("normal", Mat4<real>::transpose(knotTransform->worldToLocalMatrix()));

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
        }
    }

  private:
    GameObject* knot = nullptr;
    real sensitivity = 5;
    ShaderProgram* shaderProgram = nullptr;
    Camera* camera = nullptr;
    Transform* knotTransform = nullptr;
    Transform* cameraTransform = nullptr;
    RasterizerDescription rasterizerDesc{};
};

bool init() {
    PlatformParams params{1024, 800, "06-skybox", "GLaDOS", false};
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
