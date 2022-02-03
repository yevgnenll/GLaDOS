#ifndef GLADOS_GAMEOBJECT_HPP
#define GLADOS_GAMEOBJECT_HPP

#include <typeindex>
#include <typeinfo>

#include "Component.h"
#include "Message.h"
#include "Object.h"
#include "utils/Enumeration.h"
#include "utils/Utility.h"
#include "Cloneable.h"

namespace GLaDOS {
    class Logger;
    class Scene;
    class Transform;
    class Component;
    class GameObject : public Object, Cloneable<GameObject> {
        friend class Scene;
        friend class Transform;

      public:
        GameObject(std::string name, Scene* scene);
        GameObject(std::string name, GameObject* parent, Scene* scene);
        ~GameObject() override;

        GameObject(const GameObject& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;

        template <typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>,
                  typename = typename std::enable_if_t<std::is_convertible_v<const volatile T*, const volatile Component*>>,
                  typename... Ts>
        T* addComponent(Ts... args);
        template <typename T>
        T* getComponent();
        template <typename T>
        T* getComponentInChildren();
        template <typename T>
        T* getComponentInParent();

        void setActiveRecursive(bool value);

        template <typename T>
        bool removeComponent();
        template <typename T>
        bool sendMessage(Message& msg);
        void sendMessageUpwards(Message& msg);
        void broadcastMessage(Message& msg);
        Transform* transform();
        Scene* scene();
        uint32_t getLayer() const;
        void setLayer(uint32_t layer);
        Vector<GameObject*> getChildren() const;

      protected:
        GameObject* clone() override;
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;

      private:
        static Logger* logger;
        GameObject(GameObject* parent, Scene* scene);

        template <typename T>
        bool subscribeToMessageType(MessageType type);
        template <typename T>
        bool subscribeAllMessageType();

        Scene* mScene{nullptr};
        GameObject* mParent{nullptr};
        Transform* mTransform{nullptr};
        Set<Component*> mSubscriber[MessageType::size()];
        UnorderedMap<std::type_index, Component*> mComponents;
        Vector<GameObject*> mChildren;
        uint32_t mLayer{0}; // default layer 0
    };

    template <typename T, typename, typename, typename... Ts>
    T* GameObject::addComponent(Ts... args) {
        // duplicate key check by runtime RTTI
        if (mComponents.find(std::type_index(typeid(T))) != mComponents.end()) {
            return static_cast<T*>(nullptr);
        }

        T* component = static_cast<T*>(MALLOC(sizeof(T)));
        if (!component) {
            return static_cast<T*>(nullptr);
        }
        component->mGameObject = this;
        new (component) T(args...);
        mComponents.try_emplace(std::type_index(typeid(T)), component);

        return component;
    }

    template <typename T>
    T* GameObject::getComponent() {
        auto iter = mComponents.find(std::type_index(typeid(T)));
        if (iter == mComponents.end()) {
            return static_cast<T*>(nullptr);
        }
        return static_cast<T*>(iter->second);
    }

    template <typename T>
    T* GameObject::getComponentInChildren() {
        for (auto& gameObject : mChildren) {
            T* value = gameObject->getComponent<T>();
            if (value) {
                return value;
            }
        }

        return static_cast<T*>(nullptr);
    }

    template <typename T>
    T* GameObject::getComponentInParent() {
        GameObject* iter = mParent;

        while (iter != nullptr) {
            T* value = (*iter).getComponent<T>();
            if (value) {
                return value;
            }
            iter = iter->mParent;
        }

        return static_cast<T*>(nullptr);
    }

    template <typename T>
    bool GameObject::removeComponent() {
        auto iter = mComponents.find(std::type_index(typeid(T)));
        if (iter == mComponents.end()) {
            LOG_WARN(logger, "can't find component `{0}`", typeid(T).name());
            return false;
        }

        if (iter->second) {
            iter->second->~Component();
            FREE(iter->second);
            iter->second = nullptr;
        }
        mComponents.erase(iter);
        return true;
    }

    template <typename T>
    bool GameObject::sendMessage(Message& msg) {
        auto ret = getComponent<T>();
        if (!ret) {
            LOG_WARN(logger, "failed to send message type `{0}` in game object `{1}`", msg.type(), mName);
            return false;
        }
        MessageType type = msg.type();
        for (const auto& component : mSubscriber[type]) {
            if (component == ret) {
                component->handleMessage(msg);
                break;
            }
        }
        return true;
    }

    template <typename T>
    bool GameObject::subscribeToMessageType(MessageType type) {
        auto ret = getComponent<T>();
        if (!ret) {
            LOG_WARN(logger, "failed to subscribe message type `{0}` in game object `{1}`", type, mName);
            return false;
        }
        return mSubscriber[type].insert(ret).second;
    }

    template <typename T>
    bool GameObject::subscribeAllMessageType() {
        auto ret = getComponent<T>();
        if (!ret) {
            LOG_WARN(logger, "failed to subscribe all message in game object `{1}`", mName);
            return false;
        }
        for (auto& subscriber : mSubscriber) {
            subscriber.insert(ret);
        }
        return true;
    }
}  // namespace GLaDOS

#endif  //GLADOS_GAMEOBJECT_HPP
