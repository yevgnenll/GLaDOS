#ifndef GLADOS_ENUMERATION_H
#define GLADOS_ENUMERATION_H

#include <chrono>

namespace GLaDOS {
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
    Undefined = -1,
    ShaderProgram = 0,
    Sound = 1,
    Texture2D = 2
  };

  enum class ShaderType {
    Unknown = -1,
    VertexShader,
    FragmentShader,
    GeometryShader,
    TessControlShader,
    TessEvalShader,
    ComputeShader,
    NumberOfShaderType
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

  enum class VertexSemantic {
    Unknown = -1,
    Position,
    Normal,
    Color,
    TexCoord0,
    TexCoord1,
    TexCoord2,
    TexCoord3,
    Tangent,
    BiTangent,
    NumberOfSemantic
  };

  enum class VertexAttributeType {
    Unknown = -1,
    Float,
    Float2,
    Float3,
    Float4,
    Half,
    Half2,
    Half3,
    Half4,
    Int,
    Int2,
    Int3,
    Int4,
    Byte,
    Byte2,
    Byte3,
    Byte4,
    UByte,
    UByte2,
    UByte3,
    UByte4,
    UByteNorm,
    UByte4Norm,
    Short2,
    Short4,
    UShort2,
    UShort2Norm,
    Uint,
    UShortNorm,
    UShort4Norm,
    NumberOfAttributeType
  };

  enum class UniformType {
    Unknown = -1,
    Bool,
    Int,
    UInt,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat4,
    Texture,
    Sampler,
    NumberOfUniformType
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
}  // namespace GLaDOS

#endif
