#include "FrameBuffer.h"

namespace GLaDOS {
  Color FrameBuffer::getClearColor() const {
    return mClearColor;
  }

  void FrameBuffer::setClearColor(const Color& clearColor) {
    mClearColor = clearColor;
  }
}  // namespace GLaDOS