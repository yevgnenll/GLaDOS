#ifndef GLADOS_D3DX12FRAMEBUFFER_H
#define GLADOS_D3DX12FRAMEBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/d3dx12/D3DX12Renderer.h"
#include "platform/render/FrameBuffer.h"

namespace GLaDOS {
    class Logger;
    class D3DX12FrameBuffer : public FrameBuffer {
      public:
        D3DX12FrameBuffer();
        ~D3DX12FrameBuffer() override;

        void begin() override;
        void end() override;
        void makeDepthStencilTexture() override;

      private:
        static Logger* logger;
    };
}

#endif

#endif  // GLADOS_D3DX12FRAMEBUFFER_H
