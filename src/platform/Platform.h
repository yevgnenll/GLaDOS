#ifndef GAMEENGINE_PLATFORM_H
#define GAMEENGINE_PLATFORM_H

#include <string>

#include "KeyCode.h"
#include "utils/Enumeration.h"

namespace GameEngine {
  struct PlatformParams {
    int width, height;
    std::string titleName;
    bool isFullscreen{false}, isShowCursor{true};
    int multisample{1};
    WindowStyle windowStyle{EnumConstant::defaultWindowStyle};
  };

  class Platform {
    friend class Input;
    friend class WindowsPlatform;
    friend class CocoaPlatform;
    friend class XWindowPlatform;

  public:
    Platform();
    ~Platform();
    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;

    bool initialize(const PlatformParams& params);
    void render();
    void update();

    void quit();
    int getWidth() const;
    int getHeight() const;
    std::string getTitleName() const;
    bool isFullScreen() const;
    bool isRunning() const;
    int getMultisample() const;
    bool isFocused() const;

    void setWidthHeight(int width, int height);
    void setTitleName(const std::string& titleName);
    bool isShowCursor() const;
    void setIsShowCursor(bool isShowCursor);
    void setIsFullScreen(bool isFullScreen);
    void setIsFocused(bool isFocused);

    void leftMouseDown(bool isDown);
    void rightMouseDown(bool isDown);
    void middleMouseDown(bool isDown);
    KeyCode getKeyCode(unsigned short keycode) const;
    void setKeyDown(KeyCode keycode);
    void setKeyUp(KeyCode keycode);

    static Platform* getInstance();
    static std::size_t getThreadId() noexcept;
    static int getPid() noexcept;

  private:
    void registerKeyMap();

    static Platform* instance;

    int mWidth, mHeight;
    int mLastWidth, mLastHeight;
    std::string mTitleName;
    bool mIsShowCursor, mIsFullScreen, mIsRunning{true};
    int mMultisample;
    WindowStyle mWindowStyle;
    bool mIsFocused;
    bool mKeys[static_cast<int>(KeyCode::KEY_MAX)];
    KeyCode mLocalKeymap[static_cast<int>(KeyCode::KEY_MAX)];
    bool mMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];

    //    Vector3d mMousePosition;
    //    bool mIsMultisampleSupported;
  };
}  // namespace GameEngine

#endif
