#ifndef GLADOS_SCENEMANAGER_H
#define GLADOS_SCENEMANAGER_H

#include "utils/Enumeration.h"
#include "utils/Singleton.hpp"
#include "utils/Utility.h"

namespace GLaDOS {
  class Logger;
  class Scene;
  class SceneManager : public Singleton<SceneManager> {
  public:
    SceneManager() = default;
    ~SceneManager();

    template <typename T>
    Scene* createScene(const std::string& name);
    Scene* activeScene() const;
    Scene* sceneAt(uint32_t buildIndex) const;
    Scene* sceneByName(const std::string& name) const;
    bool setActiveScene(Scene* scene);
    bool loadScene(uint32_t buildIndex);
    bool loadScene(const std::string& name);
    std::size_t sceneCount() const;
    bool isValidScene() const;

    void update(real deltaTime);
    void render();

  private:
    static Logger* logger;

    UnorderedMap<uint32_t, Scene*> mScenes;
    Scene* mCurrentScene{nullptr};
    uint32_t mLastSceneCount{0};
  };

  template <typename T>
  Scene* SceneManager::createScene(const std::string& name) {
    if (sceneByName(name) != nullptr) {
      LOG_ERROR(logger, "Already exist scene name");
      return nullptr;
    }

    T* scene = static_cast<T*>(MALLOC(sizeof(T)));
    if (!scene) {
      return static_cast<T*>(nullptr);
    }
    new (scene) T{};
    scene->mBuildIndex = mLastSceneCount;
    scene->mName = name;
    LOG_TRACE(logger, "Scene {0} created with buildIndex {1}.", scene->mName, scene->mBuildIndex);

    if (!scene->onInit()) {
      LOG_TRACE(logger, "Failed to onInit in scene {0}", scene->getName());
      FREE(scene);
      return nullptr;
    }
    mScenes.try_emplace(mLastSceneCount, scene);
    mLastSceneCount++;

    return scene;
  }
}  // namespace GLaDOS

#endif  //GLADOS_SCENEMANAGER_H
