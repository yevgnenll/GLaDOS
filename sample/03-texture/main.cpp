#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
public:
  MainScene(const std::string& name) : Scene{name} {}
  ~MainScene() override {}

  bool onInit() override {
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
    VertexData* vertexData = NEW_T(VertexData(VertexFormatBuilder().withPosition().withTexCoord0(), 36));
    vertexData->uploadData(reinterpret_cast<std::byte*>(quad));
    Mesh* mesh = Platform::getRenderer()->createMesh(vertexData, nullptr, PrimitiveType::Triangle, false, false);
    if (mesh == nullptr) {
      LOG_ERROR("Mesh initialize failed!");
      return false;
    }
    shaderProgram = Platform::getRenderer()->createShaderProgram("textureVertex.metal", "textureFragment.metal", vertexData);
    if (shaderProgram == nullptr) {
      LOG_ERROR("Shader initialize failed!");
      return false;
    }
    DepthStencilDescription depthStencilDesc{};
    shaderProgram->setDepthStencilState(depthStencilDesc);

    Texture2D* cubeTexture = Platform::getRenderer()->createTexture2D("cube.png", PixelFormat::RGBA32);
    if (!cubeTexture->loadTextureFromFile()) {
      LOG_ERROR("Failed to load texture!");
      return false;
    }

    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);
    material->setTexture0(cubeTexture);

    GameObject* cube = NEW_T(GameObject("cube", this));
    planeTransform = cube->transform();
    planeTransform->setLocalScale({0.5, 0.5, 0.5});
    auto* meshRenderer = cube->addComponent<MeshRenderer>(mesh, material);

    camera = getMainCamera();
    camera->gameObject()->transform()->setLocalPosition({0, 0, 2});

    return true;
  }

  void onUpdate(real deltaTime) override {
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      Platform::getInstance()->quit();
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
  if (!Platform::getInstance()->initialize(params)) {
    LOG_ERROR("Platform initialize failed!");
    return false;
  }

  Platform::getInstance()->setClearColor(Color{0, 0, 0, 1});
  return true;
}

int main(int argc, char** argv) {
  std::atexit(&dumpMemory);
  if (!init()) return -1;
  SceneManager::getInstance()->setActiveScene(SceneManager::getInstance()->createScene<MainScene>("testScene"));
  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
  }
  return 0;
}