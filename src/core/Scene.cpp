#include "Scene.h"

#include "GameObject.hpp"
#include "component/Camera.h"

namespace GLaDOS {
  Scene::Scene(std::string name, uint32_t buildIndex) : Object{std::move(name)}, mBuildIndex{buildIndex} {
    // Every scene has at least a camera.
    auto* cameraObject = NEW_T(GameObject("MainCamera", this));
    mMainCamera = cameraObject->addComponent<Camera>();
  }

  Scene::~Scene() {
    deallocIterable(mGameObjects);
  }

  void Scene::addGameObject(GameObject* object) {
    if (object == nullptr) {
      LOG_ERROR("Null GameObject can not be added scene.");
      return;
    }
    object->mScene = this;
    mGameObjects.emplace_back(object);
  }

  uint32_t Scene::getBuildIndex() const {
    return mBuildIndex;
  }

  Camera* Scene::getMainCamera() {
    return mMainCamera;
  }

  void Scene::update(real deltaTime) {
    onUpdate();
    for (auto& gameObject : mGameObjects) {
      if (gameObject->isActive()) {
        gameObject->update(deltaTime);
      }
    }
    onLateUpdate();
  }

  void Scene::render() {
    for (auto& gameObject : mGameObjects) {
      if (gameObject->isActive()) {
        gameObject->render();
      }
    }
  }
}  // namespace GLaDOS