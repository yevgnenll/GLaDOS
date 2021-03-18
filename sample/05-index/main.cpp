#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
public:
  MainScene(const std::string& name) : Scene{name} {}
  ~MainScene() override {}

  bool onInit() override {
    real vertices[] = {
        // positions         // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left
    };
    uint16_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    VertexData* vertexData = NEW_T(VertexData(VertexFormatBuilder().withPosition().withTexCoord0(), 4));
    vertexData->uploadData(reinterpret_cast<std::byte*>(vertices));
    IndexData* indexData = NEW_T(IndexData(sizeof(uint16_t), 6));
    indexData->uploadData(reinterpret_cast<std::byte*>(indices));
    Mesh* mesh = Platform::getRenderer()->createMesh(vertexData, indexData, PrimitiveType::Triangle, false, false);
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

    Texture2D* quadTexture = Platform::getRenderer()->createTexture2D("container.jpg", TextureFormat::RGB24);
    if (!quadTexture->loadTextureFromFile()) {
      LOG_ERROR("Failed to load texture!");
      return false;
    }

    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);
    material->setTexture0(quadTexture);

    GameObject* quad = NEW_T(GameObject("quad", this));
    cubeTransform = quad->transform();
    quad->addComponent<MeshRenderer>(mesh, material);

    camera = getMainCamera();
    camera->gameObject()->transform()->setLocalPosition({0, 0, 2});

    return true;
  }

  void onUpdate(real deltaTime) override {
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      Platform::getInstance()->quit();
    }

    shaderProgram->setUniform("model", cubeTransform->localToWorldMatrix());
    shaderProgram->setUniform("view", camera->worldToCameraMatrix());
    shaderProgram->setUniform("projection", camera->projectionMatrix());

    if (Input::isMousePress(MouseButton::MOUSE_LEFT)) {
      Vec3 mouseDelta = Input::mouseDeltaPosition();
      real rotationX = mouseDelta.y * sensitivity * deltaTime;
      real rotationY = mouseDelta.x * sensitivity * deltaTime;
      cubeTransform->rotate(cubeTransform->right(), Math::toRadians(rotationX));
      cubeTransform->rotate(cubeTransform->up(), Math::toRadians(-rotationY));
    }

    if (Input::isKeyPress(KeyCode::KEY_Q)) {
      cubeTransform->scale({ 0.1, 0.1, 0.1 });
    }
    if (Input::isKeyPress(KeyCode::KEY_W)) {
      cubeTransform->scale({ -0.1, -0.1, -0.1 });
    }
  }

private:
  real sensitivity = 15;
  ShaderProgram* shaderProgram = nullptr;
  Camera* camera = nullptr;
  Transform* cubeTransform = nullptr;
};

bool init() {
  PlatformParams params{1024, 800, "05-index", "GLaDOS", false};
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