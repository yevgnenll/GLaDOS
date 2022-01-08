#ifndef GLADOS_PLATFORM_H
#define GLADOS_PLATFORM_H

#include <string>

#include "KeyCode.h"
#include "math/Vec3.h"
#include "utils/Enumeration.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
    struct PlatformParams {
        int width, height;
        std::string titleName;
        std::string name;  // TODO: engine name for logging
        bool isFullscreen{false}, isShowCursor{true};
        WindowStyle windowStyle{EnumConstant::defaultWindowStyle};
    };

    class Logger;
    class Color;
    class Renderer;
    class FrameBuffer;
    class Platform : public Singleton<Platform> {
        friend class Input;
        friend class WindowsPlatform;
        friend class CocoaPlatform;
        friend class XWindowPlatform;

      public:
        Platform();
        ~Platform() override;
        Platform(const Platform&) = delete;
        Platform& operator=(const Platform&) = delete;

        // Platform specific methods
        bool initialize(const PlatformParams& params);
        void render();
        void update();
        void setContentRect(int width, int height);
        void setTitleName(const std::string& titleName);
        void showCursor(bool isShowCursor);
        void fullScreen(bool isFullScreen);

        // common methods
        void quit();
        int getContentWidth() const;
        int getContentHeight() const;
        real getDrawableWidth() const;
        real getDrawableHeight() const;
        std::string titleName() const;
        bool isFullScreen() const;
        bool isRunning() const;
        bool isFocused() const;
        bool isOccluded() const;
        bool isShowCursor() const;
        Color clearColor() const;
        real contentScale() const;
        void setClearColor(const Color& clearColor);

        static Renderer& getRenderer();
        static std::size_t getThreadId() noexcept;
        static int getPid() noexcept;
        static std::size_t getConcurrency() noexcept;

      private:
        void registerKeyMap();
        void leftMouseDown(bool isDown);
        void rightMouseDown(bool isDown);
        void middleMouseDown(bool isDown);
        KeyCode getKeyCode(unsigned short keycode) const;
        void setKeyDown(KeyCode keycode);
        void setKeyUp(KeyCode keycode);
        void printLogo() const;

        static Logger* logger;

        int mContentWidth, mContentHeight;
        std::string mTitleName;
        bool mIsShowCursor{true}, mIsFullScreen{false}, mIsRunning{true};
        bool mIsFocused, mIsOccluded;
        bool mKeys[static_cast<int>(KeyCode::KEY_MAX)];
        KeyCode mLocalKeymap[static_cast<int>(KeyCode::KEY_MAX)];
        bool mMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
        Vec3 mMousePosition;
        FrameBuffer* mMainFrameBuffer{nullptr};
        real mContentScale{1};
        CursorMode mCursorMode;
    };
}  // namespace GLaDOS

#endif
