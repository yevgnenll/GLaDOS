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
Vec3 cameraPosition = {0, 0, 2};
int w = 1024;
int h = 800;

void update(real deltaTime) {
  if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
    Platform::getInstance()->quit();
  }
}

bool init() {
  PlatformParams params{w, h, "01-setup", "GLaDOS", false};
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

  Mat4x model;
  Mat4x view = Mat4x::lookAt(cameraPosition, Vec3::normalize(cameraPosition + Vec3::forward), UVec3::up);
  Mat4x projection = Mat4x::perspective(Math::toRadians(60.f), w / h, 0.1f, 5000.f);
  rectObject->transform()->setLocalScale({0.5, 0.5, 0.5});
  real rotateDegree = 0;
  while (Platform::getInstance()->isRunning()) {
    Platform::getInstance()->update();
    Input::getInstance()->update();
    Timer::getInstance()->update();
    shaderProgram->setUniform("brightness", 1.f);
    shaderProgram->setUniform("model", rectObject->transform()->localToWorldMatrix());
    shaderProgram->setUniform("view", view);
    shaderProgram->setUniform("projection", projection);
    update(Timer::getInstance()->deltaTime());

    real moveX = 0;
    real moveY = 0;
    real speed = 1;
    if (Input::isKeyPress(KeyCode::KEY_A)) {
      moveX = -1;
    }
    if (Input::isKeyPress(KeyCode::KEY_D)) {
      moveX = +1;
    }
    if (Input::isKeyPress(KeyCode::KEY_W)) {
      moveY = +1;
    }
    if (Input::isKeyPress(KeyCode::KEY_S)) {
      moveY = -1;
    }
    Vec3 moveDir = Vec3{moveX, moveY}.makeNormalize();
    rectObject->transform()->translate(moveDir * speed * Timer::getInstance()->deltaTime());
    rectObject->transform()->setLocalEulerAngles({0, rotateDegree, 0});
    rotateDegree += 0.005;
  }

  return true;
}

int main(int argc, char** argv) {
  init();
  dumpMemory();
  return 0;
}