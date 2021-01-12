#include "Platform.h"

#include <thread>

#include "OSTypes.h"
#ifdef PLATFORM_WINDOW
#include <process.h>
#else
#include <unistd.h>
#endif

namespace GameEngine {
  void Platform::quit() { mIsRunning = false; }

  int Platform::getWidth() const { return mWidth; }

  int Platform::getHeight() const { return mHeight; }

  std::string Platform::getTitleName() const { return mTitleName; }

  bool Platform::isShowCursor() const { return mIsShowCursor; }

  bool Platform::isFullScreen() const { return mIsFullScreen; }

  bool Platform::isRunning() const { return mIsRunning; }

  void Platform::setIsFocused(bool isFocused) {
    mIsFocused = isFocused;
  }

  int Platform::getMultisample() const {
    return mMultisample;
  }

  bool Platform::isFocused() const {
    return mIsFocused;
  }

  void Platform::leftMouseDown(bool isDown) {
    mMouseButtons[static_cast<int>(MouseButton::MOUSE_LEFT)] = isDown;
  }

  void Platform::rightMouseDown(bool isDown) {
    mMouseButtons[static_cast<int>(MouseButton::MOUSE_RIGHT)] = isDown;
  }

  void Platform::middleMouseDown(bool isDown) {
    mMouseButtons[static_cast<int>(MouseButton::MOUSE_MIDDLE)] = isDown;
  }

  KeyCode Platform::getKeyCode(unsigned short keycode) const {
    if (keycode >= static_cast<int>(KeyCode::KEY_MAX)) {
      return KeyCode::KEY_UNDEFINED;
    }
    return mLocalKeymap[keycode];
  }

  void Platform::setKeyDown(KeyCode keycode) {
    if (keycode != KeyCode::KEY_UNDEFINED) {
      mKeys[static_cast<int>(keycode)] = true;
    }
  }

  void Platform::setKeyUp(KeyCode keycode) {
    if (keycode != KeyCode::KEY_UNDEFINED) {
      mKeys[static_cast<int>(keycode)] = false;
    }
  }

  Platform* Platform::getInstance() { return instance; }

  std::size_t Platform::getThreadId() noexcept {
#ifdef PLATFORM_MACOS
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<std::size_t>(tid);
#elif PLATFORM_LINUX
    return static_cast<std::size_t>(::syscall(SYS_gettid));
#elif PLATFORM_WINDOW
    return static_cast<std::size_t>(::GetCurrentThreadId());
#else  // Default to standard C++11 (other Unix)
    return static_cast<std::size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
  }

  int Platform::getPid() noexcept {
#ifdef PLATFORM_WINDOW
    return static_cast<int>(GetCurrentProcessId());
#else
    return static_cast<int>(getpid());
#endif
  }
}  // namespace GameEngine
