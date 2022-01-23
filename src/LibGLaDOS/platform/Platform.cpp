#include "Platform.h"

#include <thread>

#include "OSTypes.h"
#include "platform/render/FrameBuffer.h"
#include "utils/LoggerRegistry.h"

#ifdef PLATFORM_WINDOW
#include <process.h>
#include "platform/windows/WindowsPlatform.h"
#else
#include <sys/syscall.h>
#include <unistd.h>
#define gettid() syscall(SYS_gettid)
#endif

namespace GLaDOS {
    Logger* Platform::logger = LoggerRegistry::getInstance().makeAndGetLogger("Platform");
    void Platform::quit() {
        mMainFrameBuffer->release();
        mIsRunning = false;
    }

    int Platform::getContentWidth() const { return mContentWidth; }

    int Platform::getContentHeight() const { return mContentHeight; }

    std::string Platform::titleName() const { return mTitleName; }

    bool Platform::isFullScreen() const { return mIsFullScreen; }

    bool Platform::isRunning() const { return mIsRunning; }

    bool Platform::isFocused() const { return mIsFocused; }

    bool Platform::isOccluded() const { return mIsOccluded; }

    bool Platform::isShowCursor() const { return mIsShowCursor; }

    Color Platform::clearColor() const { return mMainFrameBuffer->getClearColor(); }

    real Platform::contentScale() const { return mContentScale; }

    void Platform::setClearColor(const Color& clearColor) { mMainFrameBuffer->setClearColor(clearColor); }

    std::size_t Platform::getThreadId() noexcept {
#if defined(PLATFORM_MACOS)
        uint64_t tid;
        pthread_threadid_np(nullptr, &tid);
        return static_cast<std::size_t>(tid);
#elif defined(PLATFORM_LINUX)
        return static_cast<std::size_t>(gettid());
#elif defined(PLATFORM_WINDOW)
        return static_cast<std::size_t>(GetCurrentThreadId());
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

    std::size_t Platform::getConcurrency() noexcept {
        return std::thread::hardware_concurrency();
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

    void Platform::printLogo() const {
        printf(R"(
   ____ _          ____   ___  ____
  / ___| |    __ _|  _ \ / _ \/ ___|
 | |  _| |   / _` | | | | | | \___ \
 | |_| | |__| (_| | |_| | |_| |___) |
  \____|_____\__,_|____/ \___/|____/
    :: GLaDOS ::  (version 1.0.0))");
        printf("\n\n");
    }
}  // namespace GLaDOS
