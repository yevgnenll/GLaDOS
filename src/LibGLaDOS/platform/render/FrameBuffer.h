#ifndef GLADOS_FRAMEBUFFER_H
#define GLADOS_FRAMEBUFFER_H

#include "math/Color.h"

namespace GLaDOS {
    class FrameBuffer {
      public:
        FrameBuffer() = default;
        virtual ~FrameBuffer() = default;

        virtual void begin() = 0;
        virtual void end() = 0;
        virtual void makeDepthStencilTexture() = 0;

        Color getClearColor() const;
        void setClearColor(const Color& clearColor);
        real getWidth() const;
        real getHeight() const;

      protected:
        Color mClearColor = Color::black;
        real mWidth{0};
        real mHeight{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_FRAMEBUFFER_H
