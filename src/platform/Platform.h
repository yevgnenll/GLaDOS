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
    bool isFullscreen{false}, isShowCursor{true};
    WindowStyle windowStyle{EnumConstant::defaultWindowStyle};
  };

  class Renderer;
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

    bool initialize(const PlatformParams& params);
    void render();
    void update();

    void quit();
    int getWidth() const;
    int getHeight() const;
    std::string getTitleName() const;
    bool isFullScreen() const;
    bool isRunning() const;
    bool isFocused() const;
    bool isOccluded() const;

    void setWidthHeight(int width, int height);
    void setTitleName(const std::string& titleName);
    bool isShowCursor() const;
    void setIsShowCursor(bool isShowCursor);
    void setIsFullScreen(bool isFullScreen);
    void setIsFocused(bool isFocused);
    void setIsOccluded(bool isOccluded);

    void setMousePosition(real x, real y);
    void leftMouseDown(bool isDown);
    void rightMouseDown(bool isDown);
    void middleMouseDown(bool isDown);
    KeyCode getKeyCode(unsigned short keycode) const;
    void setKeyDown(KeyCode keycode);
    void setKeyUp(KeyCode keycode);
    Renderer* getRenderer();

    static std::size_t getThreadId() noexcept;
    static int getPid() noexcept;

  private:
    void registerKeyMap();

    int mWidth, mHeight;
    int mLastWidth, mLastHeight;
    std::string mTitleName;
    bool mIsShowCursor, mIsFullScreen, mIsRunning{true};
    WindowStyle mWindowStyle;
    bool mIsFocused, mIsOccluded;
    bool mKeys[static_cast<int>(KeyCode::KEY_MAX)];
    KeyCode mLocalKeymap[static_cast<int>(KeyCode::KEY_MAX)];
    bool mMouseButtons[static_cast<int>(MouseButton::MOUSE_BUTTON_MAX)];
    Vec3 mMousePosition;
  };
}  // namespace GLaDOS

#endif
