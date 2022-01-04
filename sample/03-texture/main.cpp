#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        // clang-format off
        real quad[] = {
            //Front
            -1.0,  1.0,  1.0, 0.25, 0.25,
            -1.0, -1.0,  1.0, 0.25, 0.50,
            1.0, -1.0,  1.0, 0.50, 0.50,
            -1.0,  1.0,  1.0, 0.25, 0.25,
            1.0, -1.0,  1.0, 0.50, 0.50,
            1.0,  1.0,  1.0, 0.50, 0.25,

            //Left
            -1.0,  1.0, -1.0, 0.00, 0.25,
            -1.0, -1.0, -1.0, 0.00, 0.50,
            -1.0, -1.0,  1.0, 0.25, 0.50,
            -1.0,  1.0, -1.0, 0.00, 0.25,
            -1.0, -1.0,  1.0, 0.25, 0.50,
            -1.0,  1.0,  1.0, 0.25, 0.25,

            //Right
            1.0,  1.0,  1.0, 0.50, 0.25,
            1.0, -1.0,  1.0, 0.50, 0.50,
            1.0, -1.0, -1.0, 0.75, 0.50,
            1.0,  1.0,  1.0, 0.50, 0.25,
            1.0, -1.0, -1.0, 0.75, 0.50,
            1.0,  1.0, -1.0, 0.75, 0.25,

            //Top
            -1.0,  1.0, -1.0, 0.25, 0.00,
            -1.0,  1.0,  1.0, 0.25, 0.25,
            1.0,  1.0,  1.0, 0.50, 0.25,
            -1.0,  1.0, -1.0, 0.25, 0.00,
            1.0,  1.0,  1.0, 0.50, 0.25,
            1.0,  1.0, -1.0, 0.50, 0.00,

            //Bottom
            -1.0, -1.0,  1.0, 0.25, 0.50,
            -1.0, -1.0, -1.0, 0.25, 0.75,
            1.0, -1.0, -1.0, 0.50, 0.75,
            -1.0, -1.0,  1.0, 0.25, 0.50,
            1.0, -1.0, -1.0, 0.50, 0.75,
            1.0, -1.0,  1.0, 0.50, 0.50,

            //Back
            1.0,  1.0, -1.0, 0.75, 0.25,
            1.0, -1.0, -1.0, 0.75, 0.50,
            -1.0, -1.0, -1.0, 1.00, 0.50,
            1.0,  1.0, -1.0, 0.75, 0.25,
            -1.0, -1.0, -1.0, 1.00, 0.50,
            -1.0,  1.0, -1.0, 1.00, 0.25
        };
        // clang-format on
        VertexBuffer* vertexData = NEW_T(VertexBuffer(VertexFormatDescriptor().position().texCoord0(), 36));
        vertexData->setBufferData(quad);
        Mesh* mesh = Platform::getRenderer().createMesh(vertexData, nullptr);
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
        planeTransform->setLocalScale({0.5, 0.5, 0.5});
        cube->addComponent<MeshRenderer>(mesh, material);

        camera = getMainCamera();
        camera->gameObject()->transform()->setLocalPosition({0, 0, 2});

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        shaderProgram->setUniform("model", planeTransform->localToWorldMatrix());
        shaderProgram->setUniform("view", camera->worldToCameraMatrix());
        shaderProgram->setUniform("projection", camera->projectionMatrix());

        if (Input::isMousePress(MouseButton::MOUSE_LEFT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            planeTransform->rotate(planeTransform->right(), Math::toRadians(rotationX));
            planeTransform->rotate(planeTransform->up(), Math::toRadians(-rotationY));
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