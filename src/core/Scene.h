#ifndef GLADOS_SCENE_H
#define GLADOS_SCENE_H

#include <cstdint>

#include "Object.h"

namespace GLaDOS {
  class SceneManager;
  class GameObject;
  class Camera;
  class Scene : public Object {
    friend class SceneManager;

  public:
    explicit Scene(std::string name, uint32_t buildIndex);
    ~Scene() override;

    void addGameObject(GameObject* object);
    uint32_t getBuildIndex() const;
    Camera* getMainCamera();

    // Only at once being called when scene object is created
    virtual void onAwake() {}
    // being called whenever scene is activated
    virtual void onStart() {}
    // every frame
    virtual void onUpdate() {}
    // last update every frame after OnUpdate called
    virtual void onLateUpdate() {}

  private:
    void update(real deltaTime) override;
    void render() override;

    SceneManager* mManager{nullptr};
    uint32_t mBuildIndex{0};
    Vector<GameObject*> mGameObjects;
    Camera* mMainCamera;
  };
}  // namespace GLaDOS

#endif  //GLADOS_SCENE_H
