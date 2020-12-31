#ifndef GAMEENGINE_ENUMERATION_H
#define GAMEENGINE_ENUMERATION_H

#include <chrono>

namespace GameEngine {
  using SystemClock = std::chrono::system_clock;
  using TimePoint = SystemClock::time_point;

  enum class WindowStyle {
    None = 0,
    Resizable = 1 << 0,
    TitleBar = 1 << 1,
    Closable = 1 << 2,
    Maximizable = 1 << 3,
    Minimizable = 1 << 4,
    Borderless = 1 << 5
  };

  enum class ShaderType {
    VertexShader,
    FragmentShader,
    GeometryShader,
    TessControlShader,
    TessEvalShader,
    ComputeShader,
  };

  enum class TimeZone {
    Local,
    UTC
  };

  enum class LogLevel {
    Trace = 0,
    Debug,
    Info,
    Warn,
    Error,
    Off,
    NumberOfLevel
  };

  static const std::string logLevelName[static_cast<int>(LogLevel::NumberOfLevel)] = {
      "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "OFF"
  };

  static const std::string monthTable[] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  inline WindowStyle operator|(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) | static_cast<int>(b)); }

  inline bool operator&(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) & static_cast<int>(b)) == b; }

  namespace EnumConstant {
    static const WindowStyle defaultWindowStyle = WindowStyle::Resizable |
                                           WindowStyle::TitleBar |
                                           WindowStyle::Closable |
                                           WindowStyle::Maximizable |
                                           WindowStyle::Minimizable;
  }
}  // namespace GameEngine

#endif
