#ifndef GLADOS_WINDOWSPLATFORM_H
#define GLADOS_WINDOWSPLATFORM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/Platform.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// https://stackoverflow.com/questions/11544073/how-do-i-deal-with-the-max-macro-in-windows-h-colliding-with-max-in-std
#undef max
#undef min

namespace GLaDOS {
    class Logger;
    class WindowsPlatform {
        friend class Platform;

      public:
        WindowsPlatform() = default;
        ~WindowsPlatform();
        WindowsPlatform(const WindowsPlatform&) = delete;
        WindowsPlatform& operator=(const WindowsPlatform&) = delete;

        bool initialize(const PlatformParams& params);

        static DWORD makeWindowStyle(WindowStyle windowStyle);
        static std::pair<int, int> centerOfScreen();
        static std::pair<int, int> getScreenSize();
        static HWND getWindowHandle();

      private:
        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

        static WindowsPlatform* windowsPlatformInstance;

        static Logger* logger;

        HINSTANCE mhInstance;
        HWND mHandle;
    };
}  // namespace GLaDOS

#endif

#endif