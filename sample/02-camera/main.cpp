#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
public:
  MainScene(const std::string& name) : Scene{name} {

  }
  ~MainScene() override {

  }

  bool onInit() override {
    shaderProgram = Platform::getRenderer()->createShaderProgram("basicVertex.metal", "basicFragment.metal");
    if (shaderProgram == nullptr) {
      LOG_ERROR("Shader initialize failed!");
      return false;
    }
    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);

    float quad[] = {
        0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
        -0.5, -0.5, 0.0, 1.0,     0.0, 1.0, 0.0, 1.0,
        -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,

        0.5,  0.5, 0.0, 1.0,     1.0, 1.0, 0.0, 1.0,
        0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
        -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,
    };
    auto* vertexData = NEW_T(VertexData(VertexFormatBuilder().withPosition().withColor(), 6));
    vertexData->uploadData(reinterpret_cast<std::byte*>(quad));
    Mesh* mesh = Platform::getRenderer()->createMesh(vertexData, nullptr, PrimitiveType::Triangle, false, false);
    if (mesh == nullptr) {
      LOG_ERROR("Mesh initialize failed!");
      return false;
    }
    Renderable* renderable = Platform::getRenderer()->createRenderable(mesh, material);
    if (renderable == nullptr) {
      LOG_ERROR("Renderable initialize failed!");
      return false;
    }
    rectObject = NEW_T(GameObject("rectObject", this));
    auto* meshRenderer = rectObject->addComponent<MeshRenderer>();
    meshRenderer->setRenderable(renderable);

    camera = getMainCamera();
    cameraTransform = camera->gameObject()->transform();
    cameraTransform->setLocalPosition({0, 0, 2});

    Input::addAxis("Forward", NEW_T(InputHandler(KeyCode::KEY_Q, KeyCode::KEY_E, 0.1)));
    Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.1)));
    Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.1)));

    return true;
  }

  void onUpdate(real deltaTime) override {
    if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
      Platform::getInstance()->quit();
    }

    rectObject->transform()->rotate(Vec3{0, 0, deltaTime * 50});

    shaderProgram->setUniform("brightness", Math::lerp(0.f, 1.f, Math::pingPong(Timer::getInstance()->elapsedTime() * 0.5f, 1.f)));
    shaderProgram->setUniform("model", rectObject->transform()->localToWorldMatrix());
    shaderProgram->setUniform("view", camera->worldToCameraMatrix());
    shaderProgram->setUniform("projection", camera->projectionMatrix());

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
  }

private:
  real sensitivity = 5;
  ShaderProgram* shaderProgram = nullptr;
  Camera* camera = nullptr;
  Transform* cameraTransform = nullptr;
  GameObject* rectObject = nullptr;
};

bool init() {
  PlatformParams params{1024, 800, "02-camera", "GLaDOS", false};
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
  static real limitFPS = 1.0 / 60.0;
  real deltaTime = 0;
  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
    deltaTime += Timer::getInstance()->deltaTime() / limitFPS;
    while (deltaTime >= 1.0) {
      // fixedUpdate();
      deltaTime--;
    }
  }

  return 0;
}