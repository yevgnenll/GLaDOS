#ifndef GLADOS_D3DX12GPUBUFFER_H
#define GLADOS_D3DX12GPUBUFFER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include "platform/render/GPUBuffer.h"
#include "platform/render/d3dx12/D3DX12Renderer.h"

namespace GLaDOS {
    class Logger;
    class D3DX12GPUBuffer : public GPUBuffer {
      public:
        D3DX12GPUBuffer(GPUBufferType type, GPUBufferUsage usage);
        ~D3DX12GPUBuffer() override;

        bool uploadData(void* data, std::size_t size) override;

      private:
        static Logger* logger;

        ComPtr<ID3D12Resource> mD3dx12Buffer;
        ComPtr<ID3D12Resource> mUploadBuffer;
    };
}

#endif

#endif  // GLADOS_D3DX12GPUBUFFER_H
