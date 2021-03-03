#include "Component.h"

namespace GLaDOS {
  Component::Component(const std::string& name) : Object{name} {
  }

  GameObject* Component::gameObject() {
    return mGameObject;
  }

  MessageResult Component::handleMessage(Message& msg) {
    return MessageResult::Ignored;
  }
}  // namespace GLaDOS