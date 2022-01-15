#include "GameObject.hpp"

#include "Scene.h"
#include "core/component/Transform.h"

namespace GLaDOS {
    Logger* GameObject::logger = LoggerRegistry::getInstance().makeAndGetLogger("GameObject");
    GameObject::GameObject(std::string name, Scene* scene) {
        mName = name;
        mTransform = addComponent<Transform>();
        if (scene != nullptr) {
            mScene = scene;
            scene->addGameObject(this);
        }
        LOG_TRACE(logger, "GameObject `{0}` created in scene `{1}`.", mName, mScene->getName());
    }

    GameObject::GameObject(std::string name, GameObject* parent, Scene* scene) : mParent{parent} {
        mName = name;
        mParent->mChildren.emplace_back(this);
        mTransform = addComponent<Transform>();
        if (scene != nullptr) {
            mScene = scene;
            scene->addGameObject(this);
        }
        LOG_TRACE(logger, "GameObject `{0}` created in scene `{1}`.", mName, mScene->getName());
    }

    GameObject::~GameObject() {
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

    void GameObject::update(real deltaTime) {
        for (auto& i : mComponents) {
            if (i.second->isActive()) {
                i.second->update(deltaTime);
            }
        }
    }

    void GameObject::render() {
        for (auto& i : mComponents) {
            if (i.second->isActive()) {
                i.second->render();
            }
        }
    }
}  // namespace GLaDOS