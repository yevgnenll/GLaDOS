#ifndef GLADOS_INPUT_H
#define GLADOS_INPUT_H

#include "KeyCode.h"
#include "math/Vec3.h"
#include "utils/Singleton.hpp"
#include "utils/Utility.h"

namespace GLaDOS {
    class InputHandler;
    class Input : public Singleton<Input> {
      public:
        Input();
        ~Input() override;
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        void update();

        static bool isKeyDown(KeyCode keyCode);
        static bool isKeyUp(KeyCode keyCode);
        static bool isKeyPress(KeyCode keyCode);

        static Vec3 mousePosition();
        static Vec3 mouseDeltaPosition();
        static bool isMouseDown(MouseButton button);
        static bool isMouseUp(MouseButton button);
        static bool isMousePress(MouseButton button);

        static bool isAnyKeyDown();
        static bool isAnyKeyUp();
        static bool isAnyKeyPress();

        static bool addAxis(const std::string& axisName, InputHandler* handler);
        static real getAxis(const std::string& axisName);
        static real getAxisRaw(const std::string& axisName);

      private:
        Map<std::string, InputHandler*> mInputHandler;
        bool mPreviousKeys[static_cast<int>(KeyCode::KEY_MAX)];
        bool mCurrentKeys[static_cast<int>(KeyCode::KEY_MAX)];
        bool mPreviousMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
        bool mCurrentMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
        Vec3 mLastMousePos;
        Vec3 mMouseDelta;
    };
}  // namespace GLaDOS

#endif
