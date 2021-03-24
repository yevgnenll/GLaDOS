#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
public:
  MainScene(const std::string& name) : Scene{name} {}
  ~MainScene() override {}

  bool onInit() override {
    TextureCube* cubemap = Platform::getRenderer()->createTextureCube("test", PixelFormat::RGBA32);
    Vector<std::string> cubemapImages = { "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png" };
    if (!cubemap->loadTextureFromFile(cubemapImages)) {
      LOG_ERROR("Failed to load texture!");
      return false;
    }

    GameObject* cubemapObject = NEW_T(GameObject("cubemap", this));
    CubemapRenderer* cubemapRenderer = cubemapObject->addComponent<CubemapRenderer>();
    cubemapRenderer->setTextureCube(cubemap);

    Mesh* mesh = MeshGenerator::generateTorusKnot(3, 4, 64, 16, 0.2);
    if (mesh == nullptr) {
      LOG_ERROR("Mesh initialize failed!");
      return false;
    }
    shaderProgram = Platform::getRenderer()->createShaderProgram("environmentVertex.metal", "environmentFragment.metal", mesh->getVertexData());
    if (shaderProgram == nullptr) {
      LOG_ERROR("Shader initialize failed!");
      return false;
    }
    DepthStencilDescription depthStencilDesc{};
    shaderProgram->setDepthStencilState(depthStencilDesc);
    shaderProgram->setRasterizerState(rasterizerDesc);

    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);
    material->setTexture0(cubemap);

    GameObject* plane = NEW_T(GameObject("plane", this));
    planeTransform = plane->transform();
    plane->addComponent<MeshRenderer>(mesh, material);

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
      Platform::getInstance()->quit();
    }

    planeTransform->rotate(Vec3{0, deltaTime * 50, 0});

    shaderProgram->setUniform("model", planeTransform->localToWorldMatrix());
    shaderProgram->setUniform("modelViewProj", planeTransform->localToWorldMatrix() * camera->worldToCameraMatrix() * camera->projectionMatrix());
    shaderProgram->setUniform("normal", Mat4x::transpose(planeTransform->worldToLocalMatrix()));
    shaderProgram->setUniform("cameraPos", cameraTransform->position());
    shaderProgram->setUniform("isWireFrameMode", rasterizerDesc.mFillMode == FillMode::Lines);

    // camera translation
    Vec3 right = cameraTransform->right();
    right *= Input::getAxis("Horizontal") * sensitivity * deltaTime;
    cameraTransform->translate(right);

    Vec3 up = cameraTransform->up();
    up *= Input::getAxis("Forward") * sensitivity * deltaTime;
    cameraTransform->translate(up);

    Vec3 forward = cameraTransform->forward();
    forward *= Input::getAxis("Vertical") * sensitivity * deltaTime;
    cameraTransform->translate(forward);

    // camera rotation
    if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
      Vec3 mouseDelta = Input::mouseDeltaPosition();
      real rotationX = mouseDelta.y * sensitivity * deltaTime;
      real rotationY = mouseDelta.x * sensitivity * deltaTime;
      cameraTransform->rotate(cameraTransform->right(), Math::toRadians(rotationX));
      cameraTransform->rotate(UVec3::up, Math::toRadians(-rotationY));
    }

    if (Input::isKeyDown(KeyCode::KEY_M)) {
      rasterizerDesc.mFillMode = FillMode::Lines;
      shaderProgram->setRasterizerState(rasterizerDesc);
    }
    if (Input::isKeyDown(KeyCode::KEY_N)) {
      rasterizerDesc.mFillMode = FillMode::Fill;
      shaderProgram->setRasterizerState(rasterizerDesc);
    }
  }

private:
  real sensitivity = 15;
  ShaderProgram* shaderProgram = nullptr;
  Camera* camera = nullptr;
  Transform* planeTransform = nullptr;
  Transform* cameraTransform = nullptr;
  RasterizerDescription rasterizerDesc{};
};

bool init() {
  PlatformParams params{1024, 800, "06-skybox", "GLaDOS", false};
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