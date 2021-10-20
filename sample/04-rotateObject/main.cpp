#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        // clang-format off
        real quad[] = {
          -1.0,  1.0,  1.0, 0.583,  0.771,  0.014, 1.0,
          -1.0, -1.0,  1.0, 0.609,  0.115,  0.436, 1.0,
          1.0, -1.0,  1.0,  0.327,  0.483,  0.844, 1.0,
          -1.0,  1.0,  1.0, 0.822,  0.569,  0.201, 1.0,
          1.0, -1.0,  1.0,  0.435,  0.602,  0.223, 1.0,
          1.0,  1.0,  1.0,  0.310,  0.747,  0.185, 1.0,

          -1.0,  1.0, -1.0, 0.597,  0.770,  0.761, 1.0,
          -1.0, -1.0, -1.0, 0.559,  0.436,  0.730, 1.0,
          -1.0, -1.0,  1.0, 0.359,  0.583,  0.152, 1.0,
          -1.0,  1.0, -1.0, 0.483,  0.596,  0.789, 1.0,
          -1.0, -1.0,  1.0, 0.559,  0.861,  0.639, 1.0,
          -1.0,  1.0,  1.0, 0.195,  0.548,  0.859, 1.0,

          1.0,  1.0,  1.0,  0.014,  0.184,  0.576, 1.0,
          1.0, -1.0,  1.0,  0.771,  0.328,  0.970, 1.0,
          1.0, -1.0, -1.0,  0.406,  0.615,  0.116, 1.0,
          1.0,  1.0,  1.0,  0.676,  0.977,  0.133, 1.0,
          1.0, -1.0, -1.0,  0.971,  0.572,  0.833, 1.0,
          1.0,  1.0, -1.0,  0.140,  0.616,  0.489, 1.0,

          -1.0,  1.0, -1.0, 0.997,  0.513,  0.064, 1.0,
          -1.0,  1.0,  1.0, 0.945,  0.719,  0.592, 1.0,
          1.0,  1.0,  1.0,  0.543,  0.021,  0.978, 1.0,
          -1.0,  1.0, -1.0, 0.279,  0.317,  0.505, 1.0,
          1.0,  1.0,  1.0,  0.167,  0.620,  0.077, 1.0,
          1.0,  1.0, -1.0,  0.347,  0.857,  0.137, 1.0,

          -1.0, -1.0,  1.0, 0.055,  0.953,  0.042, 1.0,
          -1.0, -1.0, -1.0, 0.714,  0.505,  0.345, 1.0,
          1.0, -1.0, -1.0,  0.783,  0.290,  0.734, 1.0,
          -1.0, -1.0,  1.0, 0.722,  0.645,  0.174, 1.0,
          1.0, -1.0, -1.0,  0.302,  0.455,  0.848, 1.0,
          1.0, -1.0,  1.0,  0.225,  0.587,  0.040, 1.0,

          1.0,  1.0, -1.0,  0.517,  0.713,  0.338, 1.0,
          1.0, -1.0, -1.0,  0.053,  0.959,  0.120, 1.0,
          -1.0, -1.0, -1.0, 0.393,  0.621,  0.362, 1.0,
          1.0,  1.0, -1.0,  0.673,  0.211,  0.457, 1.0,
          -1.0, -1.0, -1.0, 0.820,  0.883,  0.371, 1.0,
          -1.0,  1.0, -1.0, 0.982,  0.099,  0.879, 1.0
        };
        // clang-format on
        VertexBuffer* vertexData = NEW_T(VertexBuffer(VertexFormatDescriptor().position().color(), 36));
        vertexData->setBufferData(quad);
        Mesh* mesh = Platform::getRenderer().createMesh(vertexData, nullptr);
        if (mesh == nullptr) {
            return false;
        }
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("basicVertex", "basicFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        DepthStencilDescription desc{};
        shaderProgram->setDepthStencilState(desc);

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

        shaderProgram->setUniform("brightness", 1.f);
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
    PlatformParams params{1024, 800, "04-rotateObject", "GLaDOS", false};
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