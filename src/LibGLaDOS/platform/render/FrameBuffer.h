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
        real getFrameWidth() const;
        real getFrameHeight() const;

      protected:
        Color mClearColor = Color::black;
        real mFrameWidth{0};
        real mFrameHeight{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_FRAMEBUFFER_H
