#include "GameObject.hpp"

#include "Scene.h"
#include "core/component/Transform.h"

namespace GLaDOS {
    Logger* GameObject::logger = LoggerRegistry::getInstance().makeAndGetLogger("GameObject");
    GameObject::GameObject(GameObject* parent, Scene* scene) : mParent{parent} {
        // private constructor for easy to use in clone method (not for publicity)
        if (mParent != nullptr) {
            mParent->mChildren.emplace_back(this);
        }
        if (scene != nullptr) {
            mScene = scene;
            scene->addGameObject(this);
        }
    }

    GameObject::GameObject(std::string name, Scene* scene) {
        mName = name;
        mTransform = addComponent<Transform>();
        if (scene != nullptr) {
            mScene = scene;
            scene->addGameObject(this);
            LOG_TRACE(logger, "GameObject `{0}` created in scene `{1}`.", mName, mScene->getName());
        }
    }

    GameObject::GameObject(std::string name, GameObject* parent, Scene* scene) : mParent{parent} {
        mName = name;
        if (mParent != nullptr) {
            mParent->mChildren.emplace_back(this);
        }
        mTransform = addComponent<Transform>();
        if (scene != nullptr) {
            mScene = scene;
            scene->addGameObject(this);
            LOG_TRACE(logger, "GameObject `{0}` created in scene `{1}`.", mName, mScene->getName());
        }
    }

    GameObject::~GameObject() {
        // don't dealloc children here. because they are freed in scene destructor.
        deallocValueInMap(mComponents);
        mChildren.clear();
    }

    void GameObject::setActiveRecursive(bool value) {
        active(value);
        for (auto& i : mChildren) {
            i->setActiveRecursive(value);
        }
    }

    void GameObject::sendMessageUpwards(Message& msg) {
        MessageType type = msg.type();
        for (const auto& componentInSelf : mSubscriber[type]) {
            componentInSelf->handleMessage(msg);
        }

        GameObject* parent = mParent;
        while (parent != nullptr) {
            for (const auto& componentInParent : parent->mSubscriber[type]) {
                componentInParent->handleMessage(msg);
            }
            parent = parent->mParent;
        }
    }

    void GameObject::broadcastMessage(Message& msg) {
        MessageType type = msg.type();
        for (const auto& componentInSelf : mSubscriber[type]) {
            componentInSelf->handleMessage(msg);
        }

        if (mChildren.empty()) {
            return;
        }

        for (auto& childGameObject : mChildren) {
            for (const auto& componentInChild : childGameObject->mSubscriber[type]) {
                componentInChild->handleMessage(msg);
            }
        }
    }

    Transform* GameObject::transform() {
        return mTransform;
    }

    Scene* GameObject::scene() {
        return mScene;
    }

    uint32_t GameObject::getLayer() const {
        return mLayer;
    }

    void GameObject::setLayer(uint32_t layer) {
        if (layer >= 64) {
            LOG_ERROR(logger, "Layer should not be greater than 64 but {0}", layer);
            return;
        }

        mLayer = layer;
    }

    Vector<GameObject*> GameObject::getChildren() const {
        return mChildren;
    }

    Transform* GameObject::parent() const {
        if (mParent == nullptr) {
            return nullptr;
        }
        return mParent->mTransform;
    }

    GameObject* GameObject::clone() {
        GameObject* clone = NEW_T(GameObject(mParent, mScene));
        clone->mName = mName + " (duplicated)";
        clone->mIsActive = mIsActive;

        // clone components in game object
        for (const auto& [key, value] : mComponents) {
            Component* component = value->clone();
            component->mGameObject = clone;
            clone->mComponents.try_emplace(std::type_index(typeid(*value)), component);
        }
        clone->mTransform = clone->getComponent<Transform>();

        // clone children of game object `recursively`
        // TODO: fix clone bug
        for (GameObject* children : mChildren) {
            clone->mChildren.emplace_back(children->clone());
        }

        // clone subscriber set
        for (std::size_t i = 0; i < MessageType::size(); i++) {
            for (const auto& comp : mSubscriber[i]) {
                auto iter = clone->mComponents.find(std::type_index(typeid(*comp)));
                if (iter == clone->mComponents.end()) {
                    LOG_WARN(logger, "miss out subscriber component `{0}`", iter->second->mName);
                    continue;
                }
                clone->mSubscriber[i].insert(iter->second);
            }
        }

        // and last copy layer value
        clone->mLayer = mLayer;

        return clone;
    }

    void GameObject::fixedUpdate(real fixedDeltaTime) {
        for (auto& comp : mComponents) {
            if (comp.second->isActive()) {
                comp.second->fixedUpdate(fixedDeltaTime);
            }
        }
    }

    void GameObject::update(real deltaTime) {
        for (auto& comp : mComponents) {
            if (comp.second->isActive()) {
                comp.second->update(deltaTime);
            }
        }
    }

    void GameObject::render() {
        for (auto& comp : mComponents) {
            if (comp.second->isActive()) {
                comp.second->render();
            }
        }
    }
}  // namespace GLaDOS