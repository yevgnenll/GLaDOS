#ifndef GAMEENGINE_ENUMERATION_H
#define GAMEENGINE_ENUMERATION_H

#include <chrono>

namespace GameEngine {
  using real = float;
  using SystemClock = std::chrono::system_clock;
  using TimePoint = SystemClock::time_point;
  using HighResolutionClock = std::chrono::high_resolution_clock;
  using HighResolutionTimePoint = std::chrono::time_point<HighResolutionClock>;
  using millisecond = std::chrono::duration<real, std::milli>;

  enum class WindowStyle {
    None = 0,
    Resizable = 1 << 0,
    TitleBar = 1 << 1,
    Closable = 1 << 2,
    Maximizable = 1 << 3,
    Minimizable = 1 << 4,
    Borderless = 1 << 5
  };

  enum class ResourceType {
    ShaderProgram = 0,
    Sound = 1,
    Texture2D = 2,
    Undefined = -1
  };

  enum class ShaderType {
    VertexShader,
    FragmentShader,
    GeometryShader,
    TessControlShader,
    TessEvalShader,
    ComputeShader
  };

  enum class BufferType {
    VertexBuffer,
    IndexBuffer,
    UniformBuffer
  };

  enum class BufferUsage {
    StreamDraw,
    StreamRead,
    StreamCopy,
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy
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

  static const char* logLevelName[static_cast<int>(LogLevel::NumberOfLevel)] = {
      "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "OFF"};

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
