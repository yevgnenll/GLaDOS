#ifndef GLADOS_COMPONENT_H
#define GLADOS_COMPONENT_H

#include "Message.h"
#include "Object.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class GameObject;
    class Component : public Object {
        friend class GameObject;

      public:
        Component(const std::string& name);
        virtual ~Component() = default;

        Component(const Component& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(Component&& other) = delete;

        GameObject* gameObject();

      protected:
        virtual void update(real deltaTime) = 0;
        virtual void render() = 0;
        virtual MessageResult handleMessage(Message& msg);

        GameObject* mGameObject;  // NOTE: do not initialize game object.
    };
}  // namespace GLaDOS

#endif  //GLADOS_COMPONENT_H
