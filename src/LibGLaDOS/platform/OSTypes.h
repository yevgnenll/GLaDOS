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
#elif defined(__APPLE__) && defined(__MACH__)

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

#define ARCH_ARM_CPU (TARGET_CPU_ARM64 || TARGET_CPU_ARM)
#define ARCH_INTEL_CPU TARGET_CPU_X86_64

#define PLATFORM_MACOS (TARGET_OS_OSX || TARGET_OS_MACCATALYST)
#define PLATFORM_IOS (TARGET_OS_IOS && !TARGET_OS_MACCATALYST)
#define PLATFORM_TVOS (TARGET_OS_TV)
#define PLATFORM_MACOS_APPLE_SILICON (PLATFORM_MACOS && ARCH_ARM_CPU)
#define PLATFORM_MACOS_INTEL (PLATFORM_MACOS && ARCH_INTEL_CPU)

#define PLATFORM_LITTLE_ENDIAN TARGET_RT_LITTLE_ENDIAN
#define PLATFORM_BIG_ENDIAN TARGET_RT_BIG_ENDIAN

#define PATH_SEPARATOR "/"

#endif

// compiler predefined macro
#if defined(_MSC_VER)
#define MSVC
#elif defined(__GCC__)
#define GCC
#if defined(__x86_64__)
#define ARCH_INTEL_CPU
#elif defined(__arm__)
#define ARCH_ARM_CPU
#endif
#elif defined(__clang__)
#define CLANG
#endif

// SIMD predefined macro
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) || (defined(__EMSCRIPTEN__) && defined(__SSE2__))
#define PLATFORM_SIMD_SSE2 1
#endif
#if defined(_M_ARM) || defined(__ARM_NEON__) || defined(__ARM_NEON)
#define PLATFORM_SIMD_NEON 1
#endif
#if defined(_M_PPC) || defined(__CELLOS_LV2__)
#define PLATFORM_SIMD_VMX 1
#endif

#endif
