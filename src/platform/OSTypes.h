#ifndef GAMEENGINE_OSTYPES_H
#define GAMEENGINE_OSTYPES_H

#if defined(_WIN32) || defined(_WIN64)
#if _WIN64
#define WINDOWS_X64
#else
#define WINDOWS_X86
#endif
#define PLATFORM_WINDOW
#define PATH_SEPARATOR "/\\"
#elif defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#define PLATFORM_MACOS
#define PATH_SEPARATOR "/"
#endif

#endif
