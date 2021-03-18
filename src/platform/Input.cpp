#include "Input.h"

#include "InputHandler.h"
#include "Platform.h"
#include "utils/Utility.h"

namespace GLaDOS {
  Input::Input() {
    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      mPreviousKeys[i] = mCurrentKeys[i] = false;
    }

    for (int i = 0; i < static_cast<int>(MouseButton::MOUSE_BUTTON_MAX); i++) {
      mPreviousMouseButtons[i] = mCurrentMouseButtons[i] = false;
    }
  }

  Input::~Input() {
    deallocValueInMap(mInputHandler);
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

      mMouseDelta = Platform::getInstance()->mMousePosition - mLastMousePos;
      mLastMousePos = Platform::getInstance()->mMousePosition;
      return;
    }

    memset(mPreviousKeys, 0, static_cast<int>(KeyCode::KEY_MAX));
    memset(mCurrentKeys, 0, static_cast<int>(KeyCode::KEY_MAX));

    memset(mPreviousMouseButtons, 0, static_cast<int>(MouseButton::MOUSE_BUTTON_MAX));
    memset(mCurrentMouseButtons, 0, static_cast<int>(MouseButton::MOUSE_BUTTON_MAX));

    memset(&mMouseDelta, 0, sizeof(Vec3));
    memset(&mLastMousePos, 0, sizeof(Vec3));
  }

  bool Input::isKeyDown(KeyCode keyCode) {
    return Input::getInstance()->mCurrentKeys[static_cast<int>(keyCode)] && !Input::getInstance()->mPreviousKeys[static_cast<int>(keyCode)];
  }

  bool Input::isKeyUp(KeyCode keyCode) {
    return !Input::getInstance()->mCurrentKeys[static_cast<int>(keyCode)] && Input::getInstance()->mPreviousKeys[static_cast<int>(keyCode)];
  }

  bool Input::isKeyPress(KeyCode keyCode) {
    return Input::getInstance()->mCurrentKeys[static_cast<int>(keyCode)];
  }

  Vec3 Input::mousePosition() {
    return Input::getInstance()->mLastMousePos;
  }

  Vec3 Input::mouseDeltaPosition() {
    return Input::getInstance()->mMouseDelta;
  }

  bool Input::isMouseDown(MouseButton button) {
    return Input::getInstance()->mCurrentMouseButtons[static_cast<int>(button)] && !Input::getInstance()->mPreviousMouseButtons[static_cast<int>(button)];
  }

  bool Input::isMouseUp(MouseButton button) {
    return !Input::getInstance()->mCurrentMouseButtons[static_cast<int>(button)] && Input::getInstance()->mPreviousMouseButtons[static_cast<int>(button)];
  }

  bool Input::isMousePress(MouseButton button) {
    return Input::getInstance()->mCurrentMouseButtons[static_cast<int>(button)];
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

  bool Input::addAxis(const std::string& axisName, InputHandler* handler) {
    if (Input::getInstance()->mInputHandler.find(axisName) != Input::getInstance()->mInputHandler.end()) {
      return false;
    }

    Input::getInstance()->mInputHandler.try_emplace(axisName, handler);
    return true;
  }

  real Input::getAxis(const std::string& axisName) {
    auto iter = Input::getInstance()->mInputHandler.find(axisName);
    if (iter == Input::getInstance()->mInputHandler.end()) {
      return real(0);
    }

    InputHandler* handler = iter->second;
    if (Input::isKeyPress(handler->positive())) {
      handler->accumulate(handler->delta());
    } else if (Input::isKeyPress(handler->negative())) {
      handler->accumulate(static_cast<real>(-1.0 * handler->delta()));
    } else {
      handler->reset();
    }

    return handler->accumulator();
  }

  real Input::getAxisRaw(const std::string& axisName) {
    auto iter = Input::getInstance()->mInputHandler.find(axisName);
    if (iter == Input::getInstance()->mInputHandler.end()) {
      return real(0);
    }

    InputHandler* handler = iter->second;
    if (Input::isKeyPress(handler->positive())) {
      return real(1);
    }
    if (Input::isKeyPress(handler->negative())) {
      return real(-1);
    }

    return real(0);
  }
}  // namespace GLaDOS
