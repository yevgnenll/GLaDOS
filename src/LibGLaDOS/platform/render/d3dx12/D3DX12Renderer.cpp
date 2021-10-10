#include "D3DX12Renderer.h"

#ifdef PLATFORM_WINDOW

#include "platform/windows/WindowsPlatform.h"

namespace GLaDOS {
    Logger* D3DX12Renderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("D3DX12Renderer");
    D3DX12Renderer::~D3DX12Renderer() {
        // Ensure that the GPU is no longer referencing resources that are about to be
        // cleaned up by the destructor.
        WaitForPreviousFrame();
        CloseHandle(mFenceEvent);
    }

    bool D3DX12Renderer::initialize(int width, int height) {
        // https://github.com/microsoft/DirectX-Graphics-Samples/tree/master/Samples/Desktop/D3D12HelloWorld
        ComPtr<IDXGIFactory4> factory;
        UINT dxgiFactoryFlags = 0;
        HRESULT hresult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        getHardwareAdapter(factory.Get(), &hardwareAdapter);

        // create device
        hresult = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // create the command queue
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        hresult = mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // create swap chain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.BufferCount = frameCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.GPUBufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> swapChain;
        hresult = factory->CreateSwapChainForHwnd(mCommandQueue.Get(), WindowsPlatform::getWindowHandle(), &swapChainDesc, nullptr, nullptr, &swapChain);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        hresult = factory->MakeWindowAssociation(WindowsPlatform::getWindowHandle(), DXGI_MWA_NO_PRINT_SCREEN);  // Prevent DXGI from responding to a print-screen key.
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        swapChain.As(&mSwapChain);
        mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

        // create descriptor heaps
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
        heapDesc.NumDescriptors = frameCount;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;  // The descriptor heap for the render-target view.
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hresult = mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRenderTargetDescHeap));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }
        mRenderTargetDescSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // create render target resources
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle{mRenderTargetDescHeap->GetCPUDescriptorHandleForHeapStart()};
        for (uint32_t n = 0; n < frameCount; n++) {
            hresult = mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n]));
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", hresultToString(hresult));
                return false;
            }
            mDevice->CreateRenderTargetView(mRenderTargets[n].Get(), nullptr, cpuDescHandle);
            cpuDescHandle.Offset(1, mRenderTargetDescSize);
        }

        // create command list
        hresult = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        hresult = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        hresult = mCommandList->Close();
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // create synchronization object
        hresult = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }
        mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (mFenceEvent == nullptr) {
            hresult = HRESULT_FROM_WIN32(GetLastError());
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", hresultToString(hresult));
                return false;
            }
        }

        LOG_TRACE(logger, "D3DX12Renderer init success");

        return true;
    }

    void D3DX12Renderer::render(Renderable* _renderable) {
        PopulateCommandList();

        // Execute the command list.
        ID3D12CommandList* ppCommandLists[] = {mCommandList.Get()};
        mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // Present the frame.
        HRESULT hresult = mSwapChain->Present(1, 0);
        if (FAILED(hresult)) {
            return;
        }

        WaitForPreviousFrame();
    }

    GPUBuffer* D3DX12Renderer::createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        return nullptr;
    }

    GPUBuffer* D3DX12Renderer::createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        return nullptr;
    }

    ShaderProgram* D3DX12Renderer::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const VertexBuffer* vertexBuffer) {
        return nullptr;
    }

    Renderable* D3DX12Renderer::createRenderable(Mesh* mesh, Material* material) {
        return nullptr;
    }

    Mesh* D3DX12Renderer::createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage) {
        return nullptr;
    }

    Mesh* D3DX12Renderer::createMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
        return nullptr;
    }

    Mesh* D3DX12Renderer::createMesh(const std::string& meshPath, PrimitiveTopology primitiveTopology, GPUBufferUsage vertexUsage, GPUBufferUsage indexUsage) {
        return nullptr;
    }

    FrameBuffer* D3DX12Renderer::createFrameBuffer() {
        return nullptr;
    }

    RenderBuffer* D3DX12Renderer::createRenderBuffer() {
        return nullptr;
    }

    DepthStencilState* D3DX12Renderer::createDepthStencilState(const DepthStencilDescription& desc) {
        return nullptr;
    }

    SamplerState* D3DX12Renderer::createSamplerState(const SamplerDescription& desc) {
        return nullptr;
    }

    RasterizerState* D3DX12Renderer::createRasterizerState(const RasterizerDescription& desc) {
        return nullptr;
    }

    Texture2D* D3DX12Renderer::createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) {
        return nullptr;
    }

    Texture2D* D3DX12Renderer::createTexture2D(const std::string& name, PixelFormat format) {
        return nullptr;
    }

    Texture2D* D3DX12Renderer::createTexture2D(PixelFormat format, Blob& data, const Color& colorKey) {
        return nullptr;
    }

    Texture2D* D3DX12Renderer::createTexture2D(PixelFormat format, Blob& data) {
        return nullptr;
    }

    Texture2D* D3DX12Renderer::createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) {
        return nullptr;
    }

    Texture3D* D3DX12Renderer::createTexture3D(const std::string& name) {
        return nullptr;
    }

    TextureCube* D3DX12Renderer::createTextureCube(const std::string& name, PixelFormat format) {
        return nullptr;
    }

    RenderTexture* D3DX12Renderer::createRenderTexture(const std::string& name) {
        return nullptr;
    }

    VertexBuffer* D3DX12Renderer::createVertexBuffer(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count) {
        return nullptr;
    }

    std::string D3DX12Renderer::hresultToString(HRESULT hr) {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
        return std::string(s_str);
    }

    void D3DX12Renderer::getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool preferHighPerfAdapter) {
        // Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
        // If no such adapter can be found, *ppAdapter will be set to nullptr.
        *ppAdapter = nullptr;

        ComPtr<IDXGIAdapter1> adapter;
        ComPtr<IDXGIFactory6> factory6;
        if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
            for (uint32_t adapterIndex = 0;
                 DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                                             adapterIndex, preferHighPerfAdapter ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED, IID_PPV_ARGS(&adapter));
                 ++adapterIndex) {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                // Check to see whether the adapter supports Direct3D 12, but don't create the actual device yet.
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                    break;
                }
            }
        } else {
            for (uint32_t adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                // Check to see whether the adapter supports Direct3D 12, but don't create the actual device yet.
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                    break;
                }
            }
        }

        *ppAdapter = adapter.Detach();
    }

    void D3DX12Renderer::PopulateCommandList() {
        // Command list allocators can only be reset when the associated
        // command lists have finished execution on the GPU; apps should use
        // fences to determine GPU execution progress.
        HRESULT hresult = mCommandAllocator->Reset();
        if (FAILED(hresult)) {
            return;
        }

        // However, when ExecuteCommandList() is called on a particular command
        // list, that command list can then be reset at any time and must be before
        // re-recording.
        hresult = mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get());
        if (FAILED(hresult)) {
            return;
        }

        // Indicate that the back buffer will be used as a render target.
        auto b = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(),
                                                      D3D12_RESOURCE_STATE_PRESENT,
                                                      D3D12_RESOURCE_STATE_RENDER_TARGET);
        mCommandList->ResourceBarrier(1, &b);

        CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle(mRenderTargetDescHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRenderTargetDescSize);

        // Record commands.
        const float clearColor[] = {0.0f, 1.0f, 1.0f, 1.0f};
        mCommandList->ClearRenderTargetView(renderTargetHandle, clearColor, 0, nullptr);

        // Indicate that the back buffer will now be used to present.
        auto b2 = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        mCommandList->ResourceBarrier(1, &b2);

        hresult = mCommandList->Close();
        if (FAILED(hresult)) {
            return;
        }
    }

    void D3DX12Renderer::WaitForPreviousFrame() {
        // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
        // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
        // sample illustrates how to use fences for efficient resource usage and to
        // maximize GPU utilization.

        // Signal and increment the fence value.
        const uint64_t fence = mFenceValue;
        HRESULT hresult = mCommandQueue->Signal(mFence.Get(), fence);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return;
        }
        mFenceValue++;

        // Wait until the previous frame is finished.
        if (mFence->GetCompletedValue() < fence) {
            hresult = mFence->SetEventOnCompletion(fence, mFenceEvent);
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", hresultToString(hresult));
                return;
            }
            WaitForSingleObject(mFenceEvent, INFINITE);
        }

        mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
    }
}  // namespace GLaDOS

#endif