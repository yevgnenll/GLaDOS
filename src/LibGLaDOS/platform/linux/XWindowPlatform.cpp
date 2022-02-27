#include "XWindowPlatform.h"

#ifdef PLATFORM_LINUX

#include <algorithm>
#include <set>
#include <limits>
#include <cstring>
#include "core/SceneManager.h"
#include "platform/Input.h"
#include "platform/Timer.h"

namespace GLaDOS {
    Logger* XWindowPlatform::logger = LoggerRegistry::getInstance().makeAndGetLogger("XWindowPlatform");
    XWindowPlatform* XWindowPlatform::xWindowPlatformInstance = nullptr;

    XWindowPlatform::~XWindowPlatform() {
        XDestroyWindow(mDisplay, mWindow);
        XCloseDisplay(mDisplay);
    }

    bool XWindowPlatform::initialize(const PlatformParams& params) {
        Platform::getInstance().printLogo();
        LOG_TRACE(logger, "Initialize XWindow Platform...");

        if (params.width <= 0 || params.height <= 0) {
            LOG_ERROR(logger, "width and height should not be less than 0.");
            return false;
        }

        if (!VulkanRenderer::getInstance().initialize(params.width, params.height)) {
            LOG_ERROR(logger, "VulkanRenderer initialize failed.");
            return false;
        }

        Platform::getInstance().registerKeyMap();

        if (!createWindow(params)) {
            return false;
        }

        return true;
    }

    bool XWindowPlatform::createWindow(const PlatformParams& params) {
        mDisplay = XOpenDisplay(nullptr);
        if (mDisplay == nullptr) {
            std::cout << "Cannot connect to X server" << std::endl;
            return false;
        }

        long visualMask = VisualScreenMask;
        XVisualInfo vInfoTemplate{};
        int numberOfVisuals;
        XVisualInfo* visualInfo = XGetVisualInfo(mDisplay, visualMask, &vInfoTemplate, &numberOfVisuals);
        Window rootWindow = RootWindow(mDisplay, vInfoTemplate.screen);
        Colormap colormap = XCreateColormap(mDisplay, rootWindow, visualInfo->visual, AllocNone);

        XSetWindowAttributes windowAttributes{};
        windowAttributes.colormap = colormap;
        windowAttributes.background_pixel = 0xFFFFFFFF;
        windowAttributes.border_pixel = 0;
        windowAttributes.event_mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;

        mWindow = XCreateWindow(mDisplay, rootWindow, 0, 0, params.width, params.height,
                                0, visualInfo->depth, InputOutput, visualInfo->visual,
                                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);
        XMapWindow(mDisplay, mWindow);
        XStoreName(mDisplay, mWindow, params.titleName.c_str());

        return true;
    }

    void XWindowPlatform::dispatchEvent() {
        XEvent event;
        while (true) {
            XNextEvent(mDisplay, &event);

            if (event.type == Expose) {
                // TODO
            }
        }
    }

    Display* XWindowPlatform::getDisplay() const {
        return mDisplay;
    }

    Window XWindowPlatform::getWindow() const {
        return mWindow;
    }

    XWindowPlatform* XWindowPlatform::getXWindowPlatform() {
        return xWindowPlatformInstance;
    }

    //////////////////////////////////////////////////////////////
    //// Platform definition
    //////////////////////////////////////////////////////////////

    Platform::Platform() {
        setDestructionPhase(4);
        XWindowPlatform::xWindowPlatformInstance = NEW_T(XWindowPlatform);
    }

    Platform::~Platform() {
        DELETE_T(XWindowPlatform::xWindowPlatformInstance, XWindowPlatform);
    }

    bool Platform::initialize(const PlatformParams& params) {
        return XWindowPlatform::xWindowPlatformInstance->initialize(params);
    }

    void Platform::render() {
        Input::getInstance().update();
    }

    void Platform::update() {
        XWindowPlatform::xWindowPlatformInstance->dispatchEvent();
    }
}

#endif