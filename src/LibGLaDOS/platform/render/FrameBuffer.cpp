#include "FrameBuffer.h"

namespace GLaDOS {
  Color FrameBuffer::getClearColor() const {
    return mClearColor;
  }

  void FrameBuffer::setClearColor(const Color& clearColor) {
    mClearColor = clearColor;
  }

  real FrameBuffer::getWidth() const {
    return mWidth;
  }

  real FrameBuffer::getHeight() const {
    return mHeight;
  }
}  // namespace GLaDOS