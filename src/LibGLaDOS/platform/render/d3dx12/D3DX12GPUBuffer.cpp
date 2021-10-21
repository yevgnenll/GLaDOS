#include "D3DX12GPUBuffer.h"

#ifdef PLATFORM_WINDOW

namespace GLaDOS {
    Logger* D3DX12GPUBuffer::logger = LoggerRegistry::getInstance().makeAndGetLogger("D3DX12GPUBuffer");
    D3DX12GPUBuffer::D3DX12GPUBuffer(GPUBufferType type, GPUBufferUsage usage) : GPUBuffer{type, usage} {}

    D3DX12GPUBuffer::~D3DX12GPUBuffer() {

    }

    bool D3DX12GPUBuffer::uploadData(void* data, std::size_t size) {
        ComPtr<ID3D12Device> device = D3DX12Renderer::getInstance().getDevice();
        if (device == nullptr) {
            LOG_ERROR(logger, "Invalid renderer device state.");
            return false;
        }
        ComPtr<ID3D12GraphicsCommandList> commandList = D3DX12Renderer::getInstance().getCommandList();
        if (commandList == nullptr) {
            LOG_ERROR(logger, "Invalid device command list.");
            return false;
        }

        // 정적인 기하구조를 위해 D3D12_HEAP_TYPE_DEFAULT 타입 힙에 생성한다.
        // D3D12_HEAP_TYPE_DEFAULT 힙은 임시 업로드용 버퍼 자원을 생성해서 복사해야한다.
        // 임시 업로드 버퍼 생성 -> 메모리에 있는 정점 자료를 업로드 버퍼에 복사 -> 업로드 버퍼를 실제 정점 버퍼로 복사

        // 정적 버퍼를 만든다.
        CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
        if (FAILED(D3DX12Renderer::getInstance().getDevice()->CreateCommittedResource(&heapProperties,D3D12_HEAP_FLAG_NONE,&resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
                                                                                      IID_PPV_ARGS(mD3dx12Buffer.GetAddressOf())))) {
            LOG_ERROR(logger, "Failed to create GPUBuffer resource.");
            return false;
        }

        // 임시 업로드 버퍼를 만든다.
        CD3DX12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        if (FAILED(D3DX12Renderer::getInstance().getDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
                                                                                      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(mUploadBuffer.GetAddressOf())))) {
            LOG_ERROR(logger, "Failed to create upload buffer resource.");
            return false;
        }

        // 버퍼에 복사할 자료를 서술한다.
        D3D12_SUBRESOURCE_DATA subResourceData{};
        subResourceData.pData = data;
        subResourceData.RowPitch = size;
        subResourceData.SlicePitch = subResourceData.RowPitch;

        // 버퍼에 자료 복사 요청을 한다.
        CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(mD3dx12Buffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
        commandList->ResourceBarrier(1, &transition);
        UpdateSubresources<1>(commandList.Get(), mD3dx12Buffer.Get(), mUploadBuffer.Get(), 0, 0, 1, &subResourceData);
        CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(mD3dx12Buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
        commandList->ResourceBarrier(1, &transition2);

        return true;
    }
}

#endif