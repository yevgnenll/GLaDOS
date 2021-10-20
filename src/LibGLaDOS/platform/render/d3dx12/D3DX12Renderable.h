#ifndef GLADOS_D3DX12RENDERABLE_H
#define GLADOS_D3DX12RENDERABLE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/d3dx12/D3DX12Renderer.h"
#include "platform/render/Renderable.h"

namespace GLaDOS {
    class Logger;
    class D3DX12Renderable : public Renderable {
      public:
        D3DX12Renderable() = default;
        ~D3DX12Renderable() override;

        void build() override;
        void bindParams() override;

      private:
        static Logger* logger;
    };
}

#endif

#endif  // GLADOS_D3DX12RENDERABLE_H
