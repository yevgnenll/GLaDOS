#ifndef GAMEENGINE_ENUMERATION_H
#define GAMEENGINE_ENUMERATION_H

namespace GameEngine {
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

  inline WindowStyle operator|(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) | static_cast<int>(b)); }

  inline bool operator&(const WindowStyle a, const WindowStyle b) { return static_cast<WindowStyle>(static_cast<int>(a) & static_cast<int>(b)) == b; }

  namespace EnumConstant {
    const WindowStyle defaultWindowStyle = WindowStyle::Resizable |
                                           WindowStyle::TitleBar |
                                           WindowStyle::Closable |
                                           WindowStyle::Maximizable |
                                           WindowStyle::Minimizable;
  }
}  // namespace GameEngine

#endif
