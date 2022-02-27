#ifndef GLADOS_XWINDOWPLATFORM_H
#define GLADOS_XWINDOWPLATFORM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_LINUX

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <vector>
#include "platform/Platform.h"

namespace GLaDOS {
    class Logger;
    class Platform;
    class XWindowPlatform {
        friend class Platform;

      public:
        XWindowPlatform() = default;
        ~XWindowPlatform();

        bool initialize(const PlatformParams& params);
        bool createWindow(const PlatformParams& params);
        void dispatchEvent();

        Display* getDisplay() const;
        Window getWindow() const;
        static XWindowPlatform* getXWindowPlatform();

      private:
        static Logger* logger;
        static XWindowPlatform* xWindowPlatformInstance;

        Display* mDisplay{nullptr};
        Window mWindow;
    };
}  // namespace GLaDOS

#endif

#endif