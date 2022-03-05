#ifndef GLADOS_CONSTANT_H
#define GLADOS_CONSTANT_H

namespace GLaDOS {
  enum class FrameFlagMask {
    FrameFocusableMask = (1 << 0),
    FrameFloatingMask = (1 << 1),
    FrameBufferedMask = (1 << 2)
  };

  inline constexpr FrameFlagMask operator|(const FrameFlagMask a, const FrameFlagMask b) { return static_cast<FrameFlagMask>(static_cast<int>(a) | static_cast<int>(b)); }
  inline bool constexpr operator&(const FrameFlagMask a, const FrameFlagMask b) { return static_cast<FrameFlagMask>(static_cast<int>(a) & static_cast<int>(b)) == b; }

  enum class FrameFlags {
    FrameRegular = static_cast<int>(FrameFlagMask::FrameFocusableMask),
    FrameTopMost = static_cast<int>(FrameFlagMask::FrameFocusableMask | FrameFlagMask::FrameFloatingMask),
    FramePopup = static_cast<int>(FrameFlagMask::FrameFloatingMask)
  };

  enum class Response {
    Ignore = 0,
    Finish = (1 << 0)
  };
}  // namespace GLaDOS::GUI

#endif  //GLADOS_CONSTANT_H
