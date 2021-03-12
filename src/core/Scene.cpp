#include "Scene.h"

#include "GameObject.hpp"
#include "component/Camera.h"

namespace GLaDOS {
  Scene::Scene(std::string name) : Object{std::move(name)} {
    // Every scene has at least a camera.
    auto* cameraObject = NEW_T(GameObject("MainCamera", this));
    mMainCamera = cameraObject->addComponent<Camera>();
    LOG_TRACE("Scene {0} created with buildIndex {1}.", mName, mBuildIndex);
  }

  Scene::~Scene() {
    deallocIterable(mGameObjects);
  }

  void Scene::addGameObject(GameObject* object) {
    if (object == nullptr) {
      LOG_ERROR("Null GameObject can not be added to scene.");
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
    onUpdate(deltaTime);
    for (auto& gameObject : mGameObjects) {
      if (gameObject->isActive()) {
        gameObject->update(deltaTime);
      }
    }
    onLateUpdate(deltaTime);
  }

  void Scene::render() {
    for (auto& gameObject : mGameObjects) {
      if (gameObject->isActive()) {
        gameObject->render();
      }
    }
  }
}  // namespace GLaDOS