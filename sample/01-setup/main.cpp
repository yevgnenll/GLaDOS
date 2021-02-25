#include "GLaDOS.h"

using namespace GLaDOS;

void update(real deltaTime) {
  if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
    Platform::getInstance()->quit();
  }

  if (Input::isKeyDown(KeyCode::KEY_A)) {
    Platform::getInstance()->fullScreen(true);
  }

  if (Input::isKeyDown(KeyCode::KEY_S)) {
    Platform::getInstance()->fullScreen(false);
  }
}

bool init() {
  PlatformParams params{800, 600, "Powered by GLaDOS", "Test", false};
  if (!Platform::getInstance()->initialize(params)) {
    LOG_ERROR("Platform initialize failed!");
    return false;
  }

  Platform::getInstance()->setClearColor(Color{1, 0, 0, 1});
  ShaderProgram* shaderProgram = Platform::getRenderer()->createShaderProgram("basicVertex.metal", "basicFragment.metal");
  if (shaderProgram == nullptr) {
    return false;
  }
  Material* material = NEW_T(Material);
  material->setShaderProgram(shaderProgram);

  // clang-format off
  float quad[] = {
      0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
      -0.5, -0.5, 0.0, 1.0,     0.0, 1.0, 0.0, 1.0,
      -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,

      0.5,  0.5, 0.0, 1.0,     1.0, 1.0, 0.0, 1.0,
      0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
      -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,
  };
// clang-format on
  auto* vertexData = NEW_T(VertexData(VertexFormatBuilder().withPosition().withColor(), 6));
  vertexData->uploadData(reinterpret_cast<std::byte*>(quad));
  Mesh* mesh = Platform::getRenderer()->createMesh(vertexData, nullptr, PrimitiveType::Triangle, false, false);
  if (mesh == nullptr) {
    return false;
  }
  Renderable* renderable = Platform::getRenderer()->createRenderable(mesh, material);
  if (renderable == nullptr) {
    return false;
  }
  auto* scene = SceneManager::getInstance()->createScene("testScene");
  SceneManager::getInstance()->setActiveScene(scene);
  GameObject* rectObject = NEW_T(GameObject("rectObject", scene));
  auto* meshRenderer = rectObject->addComponent<MeshRenderer>();
  meshRenderer->setRenderable(renderable);

  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
    Input::getInstance()->update();
    Timer::getInstance()->update();
    shaderProgram->setUniform("brightness", 0.5f * Math::cos(Timer::getInstance()->elapsedTime()) + 0.5f);
    update(Timer::getInstance()->deltaTime());
  }

  return true;
}

int main(int argc, char** argv) {
  init();
  dumpMemory();
  return 0;
}