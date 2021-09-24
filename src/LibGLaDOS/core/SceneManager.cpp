#include "SceneManager.h"

#include "Scene.h"

namespace GLaDOS {
    Logger* SceneManager::logger = LoggerRegistry::getInstance().makeAndGetLogger("SceneManager");
    SceneManager::SceneManager() {
        setDestructionPhase(3);
    }

    SceneManager::~SceneManager() {
        mCurrentScene = nullptr;
        deallocValueInMap(mScenes);
    }

    Scene* SceneManager::activeScene() const {
        return mCurrentScene;
    }

    Scene* SceneManager::sceneAt(uint32_t buildIndex) const {
        const auto iter = mScenes.find(buildIndex);
        if (iter == mScenes.end()) {
            return nullptr;
        }
        return iter->second;
    }

    Scene* SceneManager::sceneByName(const std::string& name) const {
        for (const auto& scene : mScenes) {
            if (scene.second->getName() == name) {
                return scene.second;
            }
        }

        return nullptr;
    }

    bool SceneManager::setActiveScene(Scene* scene) {
        if (scene == nullptr) {
            return false;
        }

        if (scene->onStart()) {
            LOG_TRACE(logger, "Scene {0} activated.", scene->getName());
            mCurrentScene = scene;
            return true;
        }
        LOG_TRACE(logger, "Scene {0} failed to onStart()", scene->getName());

        return false;
    }

    bool SceneManager::loadScene(uint32_t buildIndex) {
        // TODO
        return false;
    }

    bool SceneManager::loadScene(const std::string& name) {
        // TODO
        return false;
    }

    std::size_t SceneManager::sceneCount() const {
        return mScenes.size();
    }

    bool SceneManager::isValidScene() const {
        return (mCurrentScene != nullptr) && mCurrentScene->isActive();
    }

    void SceneManager::update(real deltaTime) {
        if (isValidScene()) {
            mCurrentScene->update(deltaTime);
        }
    }

    void SceneManager::render() {
        if (isValidScene()) {
            mCurrentScene->render();
        }
    }
}  // namespace GLaDOS