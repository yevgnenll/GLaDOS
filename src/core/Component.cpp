#include "Component.h"

namespace GLaDOS {
  Component::Component() : Object{"Component"} {
  }

  GameObject* Component::gameObject() {
    return mGameObject;
  }

  MessageResult Component::handleMessage(Message& msg) {
    return MessageResult::Ignored;
  }
}  // namespace GLaDOS