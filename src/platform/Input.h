#ifndef GAMEENGINE_INPUT_H
#define GAMEENGINE_INPUT_H

#include "KeyCode.h"
#include "utils/Singleton.hpp"

namespace GameEngine {
  class Input : public Singleton<Input> {
  public:
    Input();
    ~Input() override = default;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    void update();

    static bool isKeyDown(KeyCode keyCode);
    static bool isKeyUp(KeyCode keyCode);
    static bool isKeyPress(KeyCode keyCode);

    static bool isMouseDown(MouseButton button);
    static bool isMouseUp(MouseButton button);
    static bool isMousePress(MouseButton button);

    static bool isAnyKeyDown();
    static bool isAnyKeyUp();
    static bool isAnyKeyPress();

  private:
    bool mPreviousKeys[static_cast<int>(KeyCode::KEY_MAX)];
    bool mCurrentKeys[static_cast<int>(KeyCode::KEY_MAX)];

    bool mPreviousMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
    bool mCurrentMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
  };
}  // namespace GameEngine

#endif
