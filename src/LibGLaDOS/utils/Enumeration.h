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
    using OSVersion = float;

    enum class FilterMode {
        None,
        Nearest,
        Bilinear,
        Trilinear
    };

    enum class WrapMode {
        Repeat,
        Clamp,
        ClampEdge,
        ClampBorder,
        MirroredRepeat,
        MirroredClampEdge
    };

    enum class CubeMapFace {
        PositiveX = 0,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ,
        TheNumberOfFace
    };

    enum class TextureDimension {
        Unknown = -1,
        Tex1D,
        Tex2D,
        Tex3D,
        MSAATex2D,
        CubeMapTex
    };

    enum class PixelFormat {
        Unknown = -1,

        Red8,  // Single channel (R) texture format, 8 bit floating point.
        Red16,  // Single channel (R) texture format, 16 bit floating point.
        RedHalf,  // Scalar (R) texture format, 16 bit floating point.
        RedFloat,  // Scalar (R) texture format, 32 bit floating point.

        RG16,  // Two color (RG) texture format, 8-bits floating point per channel.
        RG32,  // Two channel (RG) texture format, 16 bit floating point per channel.
        RGHalf,  // Two color (RG) texture format, 16 bit floating point per channel.
        RGFloat,  // Two color (RG) texture format, 32 bit floating point per channel.

        RGB24,  // Color texture format, 8-bits per channel.
        RGBA32,  // Color with alpha texture format, 8-bits per channel.
        BGRA32,  // Color with alpha texture format, 8-bits per channel.
        RGBA64,  // Four channel (RGBA) texture format, 16 bit floating point per channel.
        RGBAHalf,  // RGB color and alpha texture format, 16 bit floating point per channel.
        RGBAFloat,  // RGB color and alpha texture format, 32-bit floats per channel.

        Alpha8,  // Alpha-only texture format, 8 bit integer.
        sRGB24,
        sRGBA32,
        Depth32,
        Stencil8,
        Depth24Stencil8,
        Depth32Stencil8
    };

    enum class TextureUsage {
        Unknown = -1,
        ShaderRead,
        ShaderWrite,
        RenderTarget
    };

    enum class FillMode {
        Fill = 0,
        Lines
    };

    enum class CullMode {
        None = 0,
        Front,
        Back
    };

    enum class WindingMode {
        ClockWise = 0,
        CounterClockWise
    };

    enum class ComparisonFunction {
        Never = 0,
        Less,
        LessEqual,
        Equal,
        Greater,
        GreaterEqual,
        Always,
        NotEqual,
        TheNumberOfCompFunc
    };

    enum class StencilOperator {
        Keep = 0,  // 현재 저장된 stencil 값을 유지
        Zero,  // stencil 값을 0으로 설정
        Replace,  // stencil 값을 Ref 값으로 변경
        Increase,  // Clamp(최대값, stencil + 1)
        IncreaseWrap,  // Increase와 같지만 최대값보다 크면 0으로 되돌림
        Decrease,  // Clamp(최소값, stencil - 1)
        DecreaseWrap,  // Decrease와 같지만 0 보다 작으면 최소값으로 되돌림
        Invert,  // 현재 stencil 값의 비트를 뒤집음
        TheNumberOfStencilOp
    };

    enum class Space {
        Self,
        World
    };

    enum class CursorMode {
        Show,
        Hidden
    };

    enum class PrimitiveTopology {
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
        Buffer,
        ShaderProgram,
        Sound,
        Texture,
        Mesh,
        Font
    };

    enum class GPUBufferType {
        VertexBuffer,
        IndexBuffer,
        UniformBuffer
    };

    // TODO
    enum class GPUBufferUsage {
        Private,  // Only accessible by the GPU
        Synchronized,  // CPU and GPU maintain their copies of resource, and must be explicitly synchronized
        Shared  // stored in system memory and is accessible to both the CPU and GPU
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
        TexCoord4,
        TexCoord5,
        TexCoord6,
        TexCoord7,
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

        UInt,
        UInt2,
        UInt3,
        UInt4,

        Byte,
        Byte2,
        Byte3,
        Byte4,

        UByte,
        UByte2,
        UByte3,
        UByte4,

        Short,
        Short2,
        Short3,
        Short4,

        UShort,
        UShort2,
        UShort3,
        UShort4,

        NumberOfAttributeType
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

    enum class MessageResult {
        False = 0,
        True,
        Ignored,
        Error
    };

    enum class AnimationWrapMode {
        Once = 0,
        Loop,
        ClampForever,
        PingPong
    };

    enum class AnimationBlendMode {
        Blend = 0,
        Additive
    };

    enum class JsonNodeType {
        Null = 0,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

    enum class JsonTokenType {
        StartObject = '{',
        EndObject = '}',
        StartArray = '[',
        EndArray = ']',
        DoubleQuote = '\"',
        Colon = ':',
        Comma = ','
    };

    class UniformType {
      public:
        enum Value : int8_t {
            Unknown = -1,
            Bool,
            Int,
            UInt,
            Float,
            Vec2,
            Vec3,
            Vec4,
            Mat2,
            Mat3,
            Mat4,
            Texture,
            Sampler,
            TheNumberOfUniformType
        };

        UniformType() = delete;
        constexpr UniformType(Value _value) : value{_value} {}  // no explicit

        constexpr operator Value() const { return value; }  // no explicit
        explicit operator bool() = delete;
        constexpr const char* toString() const {
            switch (value) {
                case Bool:
                    return "Bool";
                case Int:
                    return "Int";
                case UInt:
                    return "UInt";
                case Float:
                    return "Float";
                case Vec2:
                    return "Vec2";
                case Vec3:
                    return "Vec3";
                case Vec4:
                    return "Vec4";
                case Mat2:
                    return "Mat2";
                case Mat3:
                    return "Mat3";
                case Mat4:
                    return "Mat4";
                case Texture:
                    return "Texture";
                case Sampler:
                    return "Sampler";
                default:
                    return "Undefined";
            }
        }
        constexpr static std::size_t size() {
            return TheNumberOfUniformType;
        }

      private:
        Value value;
    };

    class ShaderType {
      public:
        enum Value : int8_t {
            Unknown = -1,
            VertexShader,
            FragmentShader,
            GeometryShader,
            TessControlShader,
            TessEvalShader,
            ComputeShader,
            TheNumberOfShaderType
        };

        ShaderType() = delete;
        constexpr ShaderType(Value _value) : value{_value} {}  // no explicit

        constexpr operator Value() const { return value; }  // no explicit
        explicit operator bool() = delete;
        constexpr const char* toString() const {
            switch (value) {
                case VertexShader:
                    return "Vertex shader";
                case FragmentShader:
                    return "Fragment shader";
                case GeometryShader:
                    return "Geometry shader";
                case TessControlShader:
                    return "Tessellation control shader";
                case TessEvalShader:
                    return "Tessellation evaluation shader";
                case ComputeShader:
                    return "Compute shader";
                default:
                    return "Undefined";
            }
        }
        constexpr static std::size_t size() {
            return TheNumberOfShaderType;
        }

      private:
        Value value;
    };

    class MessageType {
      public:
        enum Value : uint16_t {
            Undefined = 0,
            OnCollisionEnter,
            OnCollisionExit,
            OnCollisionStay,
            TheNumberOfMessageType
        };

        MessageType() = delete;
        constexpr MessageType(Value _value) : value{_value} {}  // no explicit

        constexpr operator Value() const { return value; }  // no explicit
        explicit operator bool() = delete;
        constexpr bool operator==(const MessageType& other) const { return value == other.value; }
        constexpr bool operator!=(const MessageType& other) const { return value != other.value; }
        bool operator>=(const MessageType& other) const { return value >= other.value; }
        constexpr bool operator>(const MessageType& other) const { return value > other.value; }
        constexpr bool operator<=(const MessageType& other) const { return value <= other.value; }
        constexpr bool operator<(const MessageType& other) const { return value < other.value; }

        constexpr static std::size_t size() {
            return TheNumberOfMessageType;
        }

      private:
        Value value;
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
        constexpr LogLevel(Value _value) : value{_value} {}  // no explicit

        constexpr operator Value() const { return value; }  // no explicit
        explicit operator bool() = delete;
        constexpr bool operator==(const LogLevel& other) const { return value == other.value; }
        constexpr bool operator!=(const LogLevel& other) const { return value != other.value; }
        bool operator>=(const LogLevel& other) const { return value >= other.value; }
        constexpr bool operator>(const LogLevel& other) const { return value > other.value; }
        constexpr bool operator<=(const LogLevel& other) const { return value <= other.value; }
        constexpr bool operator<(const LogLevel& other) const { return value < other.value; }

        constexpr const char* toString() const {
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

        constexpr static std::size_t size() {
            return NumberOfLevel;
        }

      private:
        Value value;
    };

    inline WindowStyle operator|(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) | static_cast<int>(b)); }
    inline bool operator&(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) & static_cast<int>(b)) == b; }
    inline TextureUsage operator|(const TextureUsage a, const TextureUsage b) { return static_cast<TextureUsage>(static_cast<int>(a) | static_cast<int>(b)); }
    inline bool operator&(const TextureUsage a, const TextureUsage b) { return static_cast<TextureUsage>(static_cast<int>(a) & static_cast<int>(b)) == b; }

    namespace EnumConstant {
        static const WindowStyle defaultWindowStyle = WindowStyle::Resizable |
                                                      WindowStyle::TitleBar |
                                                      WindowStyle::Closable |
                                                      WindowStyle::Maximizable |
                                                      WindowStyle::Minimizable;
    }
}  // namespace GLaDOS

#endif
