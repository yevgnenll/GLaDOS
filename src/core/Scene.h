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
    explicit Scene(std::string name);
    ~Scene() override;

    void addGameObject(GameObject* object);
    uint32_t getBuildIndex() const;
    Camera* getMainCamera();

    // Only at once being called when scene object is created
    virtual bool onInit() { return true; }
    // being called whenever scene is activated
    virtual bool onStart() { return true; }
    // every frame
    virtual void onUpdate(real deltaTime) {}
    // last update every frame after OnUpdate called
    virtual void onLateUpdate(real deltaTime) {}
    // fixed time update (TODO)
    virtual void fixedUpdate() {}

  private:
    void update(real deltaTime) override;
    void render() override;

    uint32_t mBuildIndex{0};
    Vector<GameObject*> mGameObjects;
    Camera* mMainCamera;
  };
}  // namespace GLaDOS

#endif  //GLADOS_SCENE_H
