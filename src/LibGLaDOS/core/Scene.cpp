#include "Scene.h"

#include "GameObject.hpp"
#include "core/component/Camera.h"

namespace GLaDOS {
  Scene::Scene() {
    // Every scene has at least a camera.
    GameObject* cameraObject = createGameObject("MainCamera");
    mMainCamera = cameraObject->addComponent<Camera>();
  }

  Scene::~Scene() {
    deallocIterable(mGameObjects);
  }

  void Scene::addGameObject(GameObject* object) {
    if (object == nullptr) {
      LOG_ERROR("default", "Null GameObject can not be added to scene.");
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

  GameObject* Scene::createGameObject(std::string name) {
    return NEW_T(GameObject(name, this));
  }

  GameObject* Scene::createGameObject(std::string name, GameObject* parent) {
    return NEW_T(GameObject(name, parent, this));
  }

  bool Scene::destroy(GameObject* gameObject) {
    if (gameObject == nullptr) {
      return false;
    }

    Vector<GameObject*>::iterator iter = std::find_if(mGameObjects.begin(), mGameObjects.end(), [gameObject](GameObject* object) {
      return object->getInstanceId() == gameObject->getInstanceId();
    });

    if (iter == mGameObjects.end()) {
      LOG_INFO("No GameObject name: {0}, instanceId: {1} found.", gameObject->getName(), gameObject->getInstanceId());
      return false;
    }

    mGameObjects.erase(iter);
    (*iter)->onDestroy();
    DELETE_T((*iter), GameObject);

    return true;
  }

  GameObject* Scene::instantiate(GameObject* original) {

  }

  GameObject* Scene::instantiate(GameObject* original, const Vec3& position) {

  }

  GameObject* Scene::instantiate(GameObject* original, const Vec3& position, const Quat& rotation) {

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