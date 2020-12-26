#include "Input.h"

#include "Platform.h"

namespace GameEngine {
  Input* Input::instance = nullptr;

  Input::Input() {
    instance = this;

    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      mPreviousKeys[i] = mCurrentKeys[i] = false;
    }

    for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
      mPreviousMouseButtons[i] = mCurrentMouseButtons[i] = false;
    }
  }

  Input::~Input() {
  }

  void Input::update() {
    if (Platform::getInstance()->isFocused()) {
      for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
        mPreviousKeys[i] = mCurrentKeys[i];
        mCurrentKeys[i] = Platform::getInstance()->mKeys[i];
      }

      for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
        mPreviousMouseButtons[i] = mCurrentMouseButtons[i];
        mCurrentMouseButtons[i] = Platform::getInstance()->mMouseButtons[i];
      }

      return;
    }
  }

  bool Input::isKeyDown(KeyCode keyCode) {
    return Input::instance->mCurrentKeys[static_cast<int>(keyCode)] && !Input::instance->mPreviousKeys[static_cast<int>(keyCode)];
  }

  bool Input::isKeyUp(KeyCode keyCode) {
    return !Input::instance->mCurrentKeys[static_cast<int>(keyCode)] && Input::instance->mPreviousKeys[static_cast<int>(keyCode)];
  }

  bool Input::isKeyPress(KeyCode keyCode) {
    return Input::instance->mCurrentKeys[static_cast<int>(keyCode)];
  }

  bool Input::isMouseDown(MouseButton button) {
    return Input::instance->mCurrentMouseButtons[static_cast<int>(button)] && !Input::instance->mPreviousMouseButtons[static_cast<int>(button)];
  }

  bool Input::isMouseUp(MouseButton button) {
    return !Input::instance->mCurrentMouseButtons[static_cast<int>(button)] && Input::instance->mPreviousMouseButtons[static_cast<int>(button)];
  }

  bool Input::isMousePress(MouseButton button) {
    return Input::instance->mCurrentMouseButtons[static_cast<int>(button)];
  }

  bool Input::isAnyKeyDown() {
    for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
      if (Input::isMouseDown(static_cast<MouseButton>(i))) {
        return true;
      }
    }

    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      if (Input::isKeyDown(static_cast<KeyCode>(i))) {
        return true;
      }
    }

    return false;
  }

  bool Input::isAnyKeyUp() {
    for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
      if (Input::isMouseUp(static_cast<MouseButton>(i))) {
        return true;
      }
    }

    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      if (Input::isKeyUp(static_cast<KeyCode>(i))) {
        return true;
      }
    }

    return false;
  }

  bool Input::isAnyKeyPress() {
    for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
      if (Input::isMousePress(static_cast<MouseButton>(i))) {
        return true;
      }
    }

    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      if (Input::isKeyPress(static_cast<KeyCode>(i))) {
        return true;
      }
    }

    return false;
  }
}  // namespace GameEngine
