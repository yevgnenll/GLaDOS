#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
public:
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
    vertexData->uploadDataNoCopy(vertices);
    IndexData* indexData = NEW_T(IndexData(sizeof(uint16_t), 6));
    indexData->uploadDataNoCopy(indices);
    Mesh* mesh = Platform::getRenderer().createMesh(vertexData, indexData);
    if (mesh == nullptr) {
      LOG_ERROR("default", "Mesh initialize failed!");
      return false;
    }
    shaderProgram = Platform::getRenderer().createShaderProgram("textureVertex.metal", "textureFragment.metal", vertexData);
    if (shaderProgram == nullptr) {
      LOG_ERROR("default", "Shader initialize failed!");
      return false;
    }
    DepthStencilDescription depthStencilDesc{};
    shaderProgram->setDepthStencilState(depthStencilDesc);
    RasterizerDescription desc;
    desc.mCullMode = CullMode::None;
    shaderProgram->setRasterizerState(desc);

    Texture2D* quadTexture = Platform::getRenderer().createTexture2D("container.jpg", PixelFormat::RGB24);
    if (!quadTexture->loadTextureFromFile()) {
      LOG_ERROR("default", "Failed to load texture!");
      return false;
    }

    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);
    material->setTexture0(quadTexture);

    GameObject* quad = createGameObject("quad");
    planeTransform = quad->transform();
    quad->addComponent<MeshRenderer>(mesh, material);

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

    if (Input::isKeyPress(KeyCode::KEY_Q)) {
      planeTransform->scale({ 0.01, 0.01, 0.01 });
    }
    if (Input::isKeyPress(KeyCode::KEY_W)) {
      planeTransform->scale({ -0.01, -0.01, -0.01 });
    }
  }

private:
  real sensitivity = 15;
  ShaderProgram* shaderProgram = nullptr;
  Camera* camera = nullptr;
  Transform* planeTransform = nullptr;
};

bool init() {
  PlatformParams params{1024, 800, "05-index", "GLaDOS", false};
  if (!Platform::getInstance().initialize(params)) {
    LOG_ERROR("default", "Platform initialize failed!");
    return false;
  }

  Platform::getInstance().setClearColor(Color{0, 0, 0, 1});
  return true;
}

int main(int argc, char** argv) {
  std::atexit(&dumpMemory);
  if (!init()) return -1;
  SceneManager::getInstance().setActiveScene(SceneManager::getInstance().createScene<MainScene>("testScene"));
  while (Platform::getInstance().isRunning()) {
    Platform::getInstance().update();
  }
  return 0;
}