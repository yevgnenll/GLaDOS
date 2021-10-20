#include "D3DX12Renderer.h"

#ifdef PLATFORM_WINDOW

#include "platform/windows/WindowsPlatform.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/d3dx12/D3DX12GPUBuffer.h"
#include "platform/render/d3dx12/D3DX12ShaderProgram.h"
#include "utils/FileSystem.h"

namespace GLaDOS {
    Logger* D3DX12Renderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("D3DX12Renderer");

    D3DX12Renderer::D3DX12Renderer() {
        setDestructionPhase(2);
    }

    D3DX12Renderer::~D3DX12Renderer() {
        // Ensure that the GPU is no longer referencing resources that are about to be
        // cleaned up by the destructor.
        WaitForPreviousFrame();
        CloseHandle(mFenceEvent);
    }

    bool D3DX12Renderer::initialize(int width, int height) {
        UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        {
            ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
                debugController->EnableDebugLayer();

                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif

        ComPtr<IDXGIFactory4> factory;
        HRESULT hresult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 하드웨어 그래픽 카드 조사
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        getHardwareAdapter(factory.Get(), &hardwareAdapter);

        // 장치 생성
        hresult = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 명령 대기열 생성
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        hresult = mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 명령 할당자 생성
        hresult = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 명령 목록 생성
        hresult = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 처음 시작할때는 닫힌 상태로 시작함. 이후 Reset()이 호출되면서 시작하게 되는데, 그러려면 먼저 닫혀있어야함.
        hresult = mCommandList->Close();
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        // 교환 사슬 서술과 생성
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.BufferCount = renderTargetCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFSDesc = {};
        swapChainFSDesc.Windowed = TRUE;

        ComPtr<IDXGISwapChain1> swapChain;
        hresult = factory->CreateSwapChainForHwnd(mCommandQueue.Get(), WindowsPlatform::getWindowHandle(), &swapChainDesc, &swapChainFSDesc, nullptr, &swapChain);
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
        // 초기 후면 버퍼 인덱스 지정
        mCurrBackBuffer = mSwapChain->GetCurrentBackBufferIndex();

        // 서술자 힙 생성
        // 서술자 힙은 서술자 종류마다 따로 만들어야함: swapChainBufferCount * RTV + DSV
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
        rtvHeapDesc.NumDescriptors = renderTargetCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;  // The descriptor heap for the render-target view.
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NodeMask = 0;
        hresult = mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRenderTargetDescHeap));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NodeMask = 0;
        hresult = mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDepthStencilDescHeap));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
        cbvHeapDesc.NumDescriptors = 10000; // TODO: 매직 넘버, 얼마나 랜더링할지 모르기 때문에 넉넉하게 잡아둠.
        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        cbvHeapDesc.NodeMask = 0;
        hresult = mDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mConstantBufferDescHeap));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }

        mRenderTargetDescSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        mDepthStencilDescSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        mConstantBufferShaderResourceDescSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // 랜더 대상 뷰(RTV) 생성
        CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHeapHandle{mRenderTargetDescHeap->GetCPUDescriptorHandleForHeapStart()};
        for (uint32_t i = 0; i < renderTargetCount; i++) {
            hresult = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i]));
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", hresultToString(hresult));
                return false;
            }
            mDevice->CreateRenderTargetView(mRenderTargets[i].Get(), nullptr, renderTargetHeapHandle);
            renderTargetHeapHandle.Offset(1, mRenderTargetDescSize);
        }

        // 깊이 스텐실 버퍼와 뷰 생성
        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0;
        depthStencilDesc.Width = width;
        depthStencilDesc.Height = height;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE optClear;
        optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        optClear.DepthStencil.Depth = 1.0F;
        optClear.DepthStencil.Stencil = 0;

        CD3DX12_HEAP_PROPERTIES heapProperties{D3D12_HEAP_TYPE_DEFAULT};
        hresult = mDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(&mDepthStencil));
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return false;
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilHeapHandle{mDepthStencilDescHeap->GetCPUDescriptorHandleForHeapStart()};
        mDevice->CreateDepthStencilView(mDepthStencil.Get(), nullptr, depthStencilHeapHandle);
        // 자원을 초기 상태에서 깊이 버퍼로 사용할 수 있는 상태로 전이한다.
        CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencil.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        mCommandList->ResourceBarrier(1, &transition);

        // 동기화 객체인 장벽 생성
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

        // 뷰포트 설정
        mScreenViewport.TopLeftX = 0.F;
        mScreenViewport.TopLeftY = 0.F;
        mScreenViewport.Width = static_cast<float>(width);
        mScreenViewport.Height = static_cast<float>(height);
        mScreenViewport.MinDepth = 0.F;
        mScreenViewport.MaxDepth = 1.F;
        mCommandList->RSSetViewports(1, &mScreenViewport);

        // 가위 직사각형 설정
        mScissorRect = { 0, 0, width, height };
        mCommandList->RSSetScissorRects(1, &mScissorRect);

        LOG_TRACE(logger, "D3DX12Renderer init success");

        return true;
    }

    void D3DX12Renderer::render(Renderable* _renderable) {
//        if (_renderable == nullptr) {
//            return;
//        }

        PopulateCommandList();

        // 명령 실행을 위해 명령 목록을 명령 대기열에 추가
        ID3D12CommandList* ppCommandLists[] = {mCommandList.Get()};
        mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // 후면 버퍼와 전면 버퍼를 교환한다.
        HRESULT hresult = mSwapChain->Present(0, 0);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return;
        }

        WaitForPreviousFrame();
    }

    GPUBuffer* D3DX12Renderer::createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* vertexBuffer = NEW_T(D3DX12GPUBuffer(GPUBufferType::VertexBuffer, usage));
        if (!vertexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create vertex buffer");
            return nullptr;
        }

        return vertexBuffer;
    }

    GPUBuffer* D3DX12Renderer::createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        GPUBuffer* indexBuffer = NEW_T(D3DX12GPUBuffer(GPUBufferType::IndexBuffer, usage));
        if (!indexBuffer->uploadData(data, size)) {
            LOG_ERROR(logger, "Failed to create index buffer");
            return nullptr;
        }

        return indexBuffer;
    }

    ShaderProgram* D3DX12Renderer::createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource, const VertexBuffer* vertexBuffer) {
        D3DX12ShaderProgram* shaderProgram = NEW_T(D3DX12ShaderProgram);
        if (!shaderProgram->createShaderProgram(vertexSource, fragmentSource, vertexBuffer)) {
            LOG_ERROR(logger, "Shader compilation error");
            return nullptr;
        }

        return shaderProgram;
    }

    ShaderProgram* D3DX12Renderer::createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, const VertexBuffer* vertexBuffer) {
        D3DX12ShaderProgram* shaderProgram = NEW_T(D3DX12ShaderProgram);
        std::string shaderDirectory = shaderProgram->directory();

        FileSystem vertexFile{shaderDirectory + vertexName + SHADER_SUFFIX, OpenMode::ReadBinary};
        std::string vertexSource;
        if (!vertexFile.readAll(vertexSource)) {
            LOG_ERROR(logger, "Vertex shader {0} is not found.", vertexFile.fullName());
            return nullptr;
        }

        FileSystem fragmentFile{shaderDirectory + fragmentName + SHADER_SUFFIX, OpenMode::ReadBinary};
        std::string fragmentSource;
        if (!fragmentFile.readAll(fragmentSource)) {
            LOG_ERROR(logger, "Fragment shader {0} is not found.", fragmentFile.fullName());
            return nullptr;
        }

        if (!shaderProgram->createShaderProgram(vertexSource, fragmentSource, vertexBuffer)) {
            LOG_ERROR(logger, "Shader compilation error");
            return nullptr;
        }

        return shaderProgram;
    }

    Renderable* D3DX12Renderer::createRenderable(Mesh* mesh, Material* material) {
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

    ComPtr<ID3D12Device> D3DX12Renderer::getDevice() const {
        return mDevice;
    }

    ComPtr<ID3D12GraphicsCommandList> D3DX12Renderer::getCommandList() const {
        return mCommandList;
    }

    std::string D3DX12Renderer::hresultToString(HRESULT hresult) {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hresult));
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
        // 명령 기록에 관련된 메모리의 재활용을 위해 명령 할당자를 재설정한다.
        HRESULT hresult = mCommandAllocator->Reset();
        if (FAILED(hresult)) {
            return;
        }

        // 명령 목록을 재설정하면 메모리가 재활용된다.
        hresult = mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get());
        if (FAILED(hresult)) {
            return;
        }

        // 뷰포트와 가위 직사각형을 설정한다.
        mCommandList->RSSetViewports(1, &mScreenViewport);
        mCommandList->RSSetScissorRects(1, &mScissorRect);

        // 후면 버퍼가 렌더 타겟으로 사용될 상태로의 전이를 통지한다.
        CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mCurrBackBuffer].Get(),
                                                      D3D12_RESOURCE_STATE_PRESENT,
                                                      D3D12_RESOURCE_STATE_RENDER_TARGET);
        mCommandList->ResourceBarrier(1, &transition);

        // 후면 버퍼와 깊이 버퍼를 지운다.
        const float clearColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
        CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle(mRenderTargetDescHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRenderTargetDescSize);
        CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilHeapHandle{mDepthStencilDescHeap->GetCPUDescriptorHandleForHeapStart()};
        mCommandList->ClearRenderTargetView(renderTargetHandle, clearColor, 0, nullptr);
        mCommandList->ClearDepthStencilView(depthStencilHeapHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.F, 0, 0, nullptr);

        // 렌더링 결과가 기록될 렌더 대상 버퍼들을 지정한다.
        mCommandList->OMSetRenderTargets(1, &renderTargetHandle, true, &depthStencilHeapHandle);

        // 실제 랜더링을 수행한다.
        ID3D12DescriptorHeap* descriptorHeaps[] = { mConstantBufferDescHeap.Get() };
        mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);



        // 후면 버퍼가 보여질 준비로 상태 전이됬음을 통지한다.
        CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mCurrBackBuffer].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        mCommandList->ResourceBarrier(1, &transition2);

        // 명령들의 기록을 마침
        hresult = mCommandList->Close();
        if (FAILED(hresult)) {
            return;
        }
    }

    void D3DX12Renderer::WaitForPreviousFrame() {
        // 명령 대기열을 flush 한다.
        // 현재 펜스까지 명령들을 표시하도록 펜스값을 전진시킨다.
        const uint64_t fence = mFenceValue;
        HRESULT hresult = mCommandQueue->Signal(mFence.Get(), fence);
        if (FAILED(hresult)) {
            LOG_ERROR(logger, "{0}", hresultToString(hresult));
            return;
        }
        mFenceValue++;

        // GPU가 이 펜스지점까지 명령들을 완료할때 까지 "기다"린다.
        if (mFence->GetCompletedValue() < fence) {
            // GPU가 현재 펜스 지점에 도달했으면 이벤트를 발동한다.
            hresult = mFence->SetEventOnCompletion(fence, mFenceEvent);
            if (FAILED(hresult)) {
                LOG_ERROR(logger, "{0}", hresultToString(hresult));
                return;
            }
            WaitForSingleObject(mFenceEvent, INFINITE);
        }

        // 후면 버퍼 인덱스를 갱신한다.
        mCurrBackBuffer = mSwapChain->GetCurrentBackBufferIndex();
    }
}  // namespace GLaDOS

#endif