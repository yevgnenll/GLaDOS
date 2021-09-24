#ifndef GLADOS_METALRENDERBUFFER_H
#define GLADOS_METALRENDERBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/RenderBuffer.h"

namespace GLaDOS {
    class MetalRenderBuffer : public RenderBuffer {
      public:
        MetalRenderBuffer() = default;
        ~MetalRenderBuffer() override = default;

        void begin() override;
        void end() override;
    };
}  // namespace GLaDOS

#endif

#endif