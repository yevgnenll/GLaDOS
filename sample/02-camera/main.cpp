#include "GLaDOS.h"

using namespace GLaDOS;

float quad[] = {
    0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5, 0.0, 1.0,     0.0, 1.0, 0.0, 1.0,
    -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,

    0.5,  0.5, 0.0, 1.0,     1.0, 1.0, 0.0, 1.0,
    0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,
};
int w = 800;
int h = 600;

void update(real deltaTime) {
  if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
    Platform::getInstance()->quit();
  }
}

bool init() {
  PlatformParams params{w, h, "02-camera", "GLaDOS", false};
  if (!Platform::getInstance()->initialize(params)) {
    LOG_ERROR("Platform initialize failed!");
    return false;
  }

  Platform::getInstance()->setClearColor(Color{0, 0, 0, 1});
  ShaderProgram* shaderProgram = Platform::getRenderer()->createShaderProgram("basicVertex.metal", "basicFragment.metal");
  if (shaderProgram == nullptr) {
    LOG_ERROR("Shader initialize failed!");
    return false;
  }
  Material* material = NEW_T(Material);
  material->setShaderProgram(shaderProgram);

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
  auto* scene = SceneManager::getInstance()->createScene("testScene");
  SceneManager::getInstance()->setActiveScene(scene);
  GameObject* rectObject = NEW_T(GameObject("rectObject", scene));
  auto* meshRenderer = rectObject->addComponent<MeshRenderer>();
  meshRenderer->setRenderable(renderable);

  auto* camera = scene->getMainCamera();
  auto* cameraTransform = camera->gameObject()->transform();
  cameraTransform->setLocalPosition({0, 0, 2});
  Mat4x model;
  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
    Input::getInstance()->update();
    Timer::getInstance()->update();
    shaderProgram->setUniform("brightness", Math::lerp(0.5f, 1.f, Math::pingPong(Timer::getInstance()->elapsedTime() * 0.5f, 1.f)));
    shaderProgram->setUniform("model", model);
    shaderProgram->setUniform("view", camera->worldToCameraMatrix());
    shaderProgram->setUniform("projection", camera->projectionMatrix());
    update(Timer::getInstance()->deltaTime());

    // camera translation
    real sensitivity = 15.0f;
    real moveX = 0;
    real moveY = 0;
    real moveZ = 0;
    real speed = 1;
    if (Input::isKeyPress(KeyCode::KEY_A)) {
      moveX = -1;
    }
    if (Input::isKeyPress(KeyCode::KEY_D)) {
      moveX = +1;
    }
    if (Input::isKeyPress(KeyCode::KEY_W)) {
      moveZ = +1;
    }
    if (Input::isKeyPress(KeyCode::KEY_S)) {
      moveZ = -1;
    }
    if (Input::isKeyPress(KeyCode::KEY_Q)) {
      moveY = +1;
    }
    if (Input::isKeyPress(KeyCode::KEY_E)) {
      moveY = -1;
    }
    Vec3 right = cameraTransform->right();
    right *= moveX * speed * Timer::getInstance()->deltaTime();
    cameraTransform->translate(right);

    Vec3 up = cameraTransform->up();
    up *= moveY * speed * Timer::getInstance()->deltaTime();
    cameraTransform->translate(up);

    Vec3 forward = cameraTransform->forward();
    forward *= moveZ * speed * Timer::getInstance()->deltaTime();
    cameraTransform->translate(forward);

    // camera rotation
    if (Input::isMousePress(MouseButton::MOUSE_RIGHT)) {
      Vec3 mouseDelta = Input::mouseDeltaPosition();
      Deg rotationY{mouseDelta.x * sensitivity * Timer::getInstance()->deltaTime() * -1};
      Deg rotationX{mouseDelta.y * sensitivity * Timer::getInstance()->deltaTime()};
      cameraTransform->rotate(UVec3::up, rotationY);
      cameraTransform->rotate(cameraTransform->right(), rotationX);
    }
  }

  return true;
}

int main(int argc, char** argv) {
  init();
  dumpMemory();
  return 0;
}