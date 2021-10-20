#include "WindowsPlatform.h"

#ifdef PLATFORM_WINDOW

#include "core/SceneManager.h"
#include "platform/Input.h"
#include "platform/Timer.h"
#include "platform/render/d3dx12/D3DX12Renderer.h"

namespace GLaDOS {
    Logger* WindowsPlatform::logger = LoggerRegistry::getInstance().makeAndGetLogger("WindowsPlatform");
    WindowsPlatform* WindowsPlatform::windowsPlatformInstance = nullptr;

    WindowsPlatform::~WindowsPlatform() {
        if (Platform::getInstance().mIsFullScreen) {
            ChangeDisplaySettings(nullptr, 0);  // back to the desktop
            ShowCursor(1);
        }
        DestroyWindow(mHandle);
        UnregisterClass(Platform::getInstance().mTitleName.c_str(), mhInstance);
    }

    bool WindowsPlatform::initialize(const PlatformParams& params) {
        Platform::getInstance().printLogo();
        LOG_TRACE(logger, "Initialize Windows Platform...");

        if (params.width <= 0 || params.height <= 0) {
            LOG_ERROR(logger, "Platform width and height should not be less than 0.");
            return false;
        }

        mhInstance = GetModuleHandle(nullptr);
        if (mhInstance == nullptr) {
            LOG_ERROR(logger, "GetModuleHandle failed.");
            return false;
        }

        DWORD style;
        DWORD mExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        if (params.windowStyle & WindowStyle::Resizable) {
            style = WS_OVERLAPPEDWINDOW;
        } else if (!(params.windowStyle & WindowStyle::Resizable)) {
            style = WS_CAPTION | WS_SYSMENU;
        }
        Platform::getInstance().mWidth = params.width;
        Platform::getInstance().mHeight = params.height;
        Platform::getInstance().mIsFullScreen = params.isFullscreen;
        Platform::getInstance().mTitleName = params.titleName;

        const char* cstrTitleName = Platform::getInstance().mTitleName.c_str();
        WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                         CS_CLASSDC,
                         WndProc,
                         0L,
                         0L,
                         mhInstance,
                         LoadIcon(mhInstance, nullptr),  // icon
                         LoadCursor(mhInstance, nullptr),  // cursor
                         nullptr,
                         nullptr,
                         cstrTitleName,
                         nullptr};
        if (!RegisterClassEx(&wc)) {
            LOG_ERROR(logger, "RegisterClassEx failed.");
            return false;
        }

        RECT rt = {0, 0, Platform::getInstance().mWidth, Platform::getInstance().mHeight};
        if (AdjustWindowRectEx(&rt, style, 0, mExStyle) == 0) {
            LOG_ERROR(logger, "AdjustWindowRectEx failed.");
            return false;
        }

        DWORD w = rt.right - rt.left;
        DWORD h = rt.bottom - rt.top;
        const auto& [x, y] = WindowsPlatform::centerOfScreen();
        mHandle = CreateWindowEx(mExStyle, cstrTitleName, cstrTitleName, style, static_cast<int>(x), static_cast<int>(y), w, h, nullptr, nullptr, mhInstance, nullptr);
        if (mHandle == nullptr) {
            return false;
        }

        if (!D3DX12Renderer::getInstance().initialize(params.width, params.height)) {
            LOG_ERROR(logger, "D3DX12Renderer initialize failed.");
            return false;
        }

        Platform::getInstance().registerKeyMap();
        Platform::getInstance().mMainFrameBuffer = D3DX12Renderer::getInstance().createFrameBuffer();

        ShowWindow(mHandle, SW_SHOWDEFAULT);
        UpdateWindow(mHandle);
        ShowCursor(static_cast<BOOL>(Platform::getInstance().mIsShowCursor));
        SetForegroundWindow(mHandle);
        SetFocus(mHandle);

        return true;
    }

    DWORD WindowsPlatform::makeWindowStyle(WindowStyle windowStyle) {
        // TODO
        return 0;
    }

    std::pair<int, int> WindowsPlatform::centerOfScreen() {
        const auto& [w, h] = WindowsPlatform::getScreenSize();
        return std::make_pair((w - Platform::getInstance().mWidth) / 2, (h - Platform::getInstance().mHeight) / 2);
    }

    std::pair<int, int> WindowsPlatform::getScreenSize() {
        int width = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
        int height = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));
        return std::make_pair(width, height);
    }

    HWND WindowsPlatform::getWindowHandle() {
        return windowsPlatformInstance->mHandle;
    }

    LRESULT CALLBACK WindowsPlatform::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_SYSCOMMAND: {  // Intercept System Commands
                switch (wParam) {  // Check System Calls
                    case SC_SCREENSAVE:  // Screensaver Trying To Start?
                    case SC_MONITORPOWER:  // Monitor Trying To Enter Powersave?
                        return 0;  // Prevent From Happening
                }
                break;
            }
            case WM_MOUSEWHEEL:
                // TODO
                break;
            case WM_SIZE:
                Platform::getInstance().mWidth = LOWORD(lParam);
                Platform::getInstance().mHeight = HIWORD(lParam);
                break;
            case WM_EXITSIZEMOVE:
                // TODO
                LOG_TRACE(logger, "window resize end");
                break;
            case WM_SETFOCUS:
                Platform::getInstance().mIsFocused = true;
                break;
            case WM_KILLFOCUS:
                Platform::getInstance().mIsFocused = false;
                break;
            case WM_CLOSE:
                PostQuitMessage(0);
                break;
            case WM_PAINT:
                Platform::getInstance().render();
                break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    //////////////////////////////////////////////////////////////
    //// Platform definition
    //////////////////////////////////////////////////////////////

    Platform::Platform() {
        setDestructionPhase(4);
        WindowsPlatform::windowsPlatformInstance = NEW_T(WindowsPlatform);
    }

    Platform::~Platform() {
        DELETE_T(WindowsPlatform::windowsPlatformInstance, WindowsPlatform);
    }

    void Platform::registerKeyMap() {
        LOG_TRACE(logger, "Register Windows key map...");
        for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
            mKeys[i] = false;
            mLocalKeymap[i] = KeyCode::KEY_UNDEFINED;
        }

        for (bool& mMouseButton : mMouseButtons) {
            mMouseButton = false;
        }

        // TODO
    }

    bool Platform::initialize(const PlatformParams& params) {
        return WindowsPlatform::windowsPlatformInstance->initialize(params);
    }

    void Platform::render() {
        // vsync with timer, input update
        Input::getInstance().update();
        Timer::getInstance().update();

        // actual rendering start
        //    mMainFrameBuffer->begin();
        //    SceneManager::getInstance().update(Timer::getInstance().deltaTime()); // should be here in scope of semaphore lock
        //    SceneManager::getInstance().render();
        //    mMainFrameBuffer->end();
        D3DX12Renderer::getInstance().render(nullptr);
    }

    void Platform::update() {
        MSG msg = {
            0,
        };
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                mIsRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        POINT mousePoint;
        GetCursorPos(&mousePoint);
        ScreenToClient(WindowsPlatform::windowsPlatformInstance->mHandle, &mousePoint);

        mMousePosition.x = (float)mousePoint.x;
        mMousePosition.y = (float)-mousePoint.y;

        mMouseButtons[static_cast<int>(MouseButton::MOUSE_LEFT)] = static_cast<bool>(GetAsyncKeyState(VK_LBUTTON));
        mMouseButtons[static_cast<int>(MouseButton::MOUSE_RIGHT)] = static_cast<bool>(GetAsyncKeyState(VK_RBUTTON));
        mMouseButtons[static_cast<int>(MouseButton::MOUSE_MIDDLE)] = static_cast<bool>(GetAsyncKeyState(VK_MBUTTON));

        for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
            mKeys[i] = static_cast<bool>(GetAsyncKeyState(static_cast<int>(mLocalKeymap[i])));
        }
    }

    void Platform::setViewport(int width, int height) {
    }

    void Platform::setTitleName(const std::string& titleName) {
        mTitleName = titleName;
        SetWindowText(WindowsPlatform::windowsPlatformInstance->mHandle, titleName.c_str());
    }

    void Platform::showCursor(bool isShowCursor) {
    }

    void Platform::fullScreen(bool isFullScreen) {
    }

    Renderer& Platform::getRenderer() {
        return D3DX12Renderer::getInstance();
    }
}  // namespace GLaDOS

#endif