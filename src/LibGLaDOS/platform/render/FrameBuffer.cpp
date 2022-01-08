#include "FrameBuffer.h"

namespace GLaDOS {
    Color FrameBuffer::getClearColor() const {
        return mClearColor;
    }

    void FrameBuffer::setClearColor(const Color& clearColor) {
        mClearColor = clearColor;
    }

    real FrameBuffer::getFrameWidth() const {
        return mFrameWidth;
    }

    real FrameBuffer::getFrameHeight() const {
        return mFrameHeight;
    }
}  // namespace GLaDOS