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

  enum class CursorMode {
    Show,
    Hidden
  };

  enum class PrimitiveType {
    Point = 0,
    Line,
    LineStrip,
    Triangle,
    TriangleStrip
  };

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

  enum class OpenMode {
    Read = 0,
    Write,
    ReadWrite,
    AppendOnly,
    ReadBinary,
    WriteBinary,
    ReadWriteBinary
  };

  class LogLevel {
  public:
    enum Value : uint8_t {
      Trace = 0,
      Debug,
      Info,
      Warn,
      Error,
      Off,
      NumberOfLevel
    };

    LogLevel() = delete;
    constexpr LogLevel(Value _value) : value{_value} {}

    constexpr operator Value() const { return value; }
    explicit operator bool() = delete;
    constexpr bool operator==(const LogLevel& other) const { return value == other.value; }
    constexpr bool operator!=(const LogLevel& other) const { return value != other.value; }
    bool operator>=(const LogLevel& other) const { return value >= other.value; }
    constexpr bool operator>(const LogLevel& other) const { return value > other.value; }
    constexpr bool operator<=(const LogLevel& other) const { return value <= other.value; }
    constexpr bool operator<(const LogLevel& other) const { return value < other.value; }

    const char* toString() const {
      switch (value) {
        case Trace:
          return "TRACE";
        case Debug:
          return "DEBUG";
        case Info:
          return "INFO";
        case Warn:
          return "WARN";
        case Error:
          return "ERROR";
        case Off:
          return "OFF";
        default:
          return "UNDEFINED";
      }
    }

    static std::size_t size() {
      return NumberOfLevel;
    }

  private:
    Value value;
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
}  // namespace GLaDOS

#endif
