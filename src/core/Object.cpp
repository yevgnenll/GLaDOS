#include "Object.h"

namespace GLaDOS {
  Object::Object(std::string name) : mName{std::move(name)} {
  }

  Object::~Object() {
    onDestroy();
  }

  std::string Object::getName() const {
    return mName;
  }

  void Object::setName(const std::string& name) {
    mName = name;
  }

  bool Object::isActive() const {
    return mIsActive;
  }

  void Object::active(bool value) {
    value ? onEnable() : onDisable();
    mIsActive = value;
  }
}  // namespace GLaDOS