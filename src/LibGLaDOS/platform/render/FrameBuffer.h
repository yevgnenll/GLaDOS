#ifndef GLADOS_FRAMEBUFFER_H
#define GLADOS_FRAMEBUFFER_H

#include "math/Color.h"
#include "utils/ReferenceCountingMixin.h"

namespace GLaDOS {
    class FrameBuffer : public ReferenceCountingMixin {
      public:
        FrameBuffer() = default;
        ~FrameBuffer() override = default;

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
