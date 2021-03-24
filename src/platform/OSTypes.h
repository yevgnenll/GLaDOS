#ifndef GLADOS_OSTYPES_H
#define GLADOS_OSTYPES_H

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
#define PATH_SEPARATOR "/"
#elif defined(__APPLE__) || defined(__MACH__)

/*
    +---------------------------------------------------------------------+
    |                            TARGET_OS_MAC                            |
    | +---+ +-----------------------------------------------+ +---------+ |
    | |   | |               TARGET_OS_IPHONE                | |         | |
    | |   | | +---------------+ +----+ +-------+ +--------+ | |         | |
    | |   | | |      IOS      | |    | |       | |        | | |         | |
    | |OSX| | |+-------------+| | TV | | WATCH | | BRIDGE | | |DRIVERKIT| |
    | |   | | || MACCATALYST || |    | |       | |        | | |         | |
    | |   | | |+-------------+| |    | |       | |        | | |         | |
    | |   | | +---------------+ +----+ +-------+ +--------+ | |         | |
    | +---+ +-----------------------------------------------+ +---------+ |
    +---------------------------------------------------------------------+
*/
#include <TargetConditionals.h>

#define PLATFORM_APPLE_SILICON (TARGET_CPU_ARM64 || TARGET_CPU_ARM)
#define PLATFORM_INTEL_CPU TARGET_CPU_X86_64

#define PLATFORM_MACOS (TARGET_OS_OSX || TARGET_OS_MACCATALYST)
#define PLATFORM_IOS (TARGET_OS_IOS && !TARGET_OS_MACCATALYST)
#define PLATFORM_TVOS TARGET_OS_TV
#define PLATFORM_MACOS_APPLE_SILICON (PLATFORM_MACOS && PLATFORM_APPLE_SILICON)
#define PLATFORM_MACOS_INTEL (PLATFORM_MACOS && PLATFORM_INTEL_CPU)

#define PLATFORM_LITTLE_ENDIAN TARGET_RT_LITTLE_ENDIAN
#define PLATFORM_BIG_ENDIAN TARGET_RT_BIG_ENDIAN

#define PATH_SEPARATOR "/"

#endif

#endif
