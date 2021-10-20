#ifndef GLADOS_D3DX12RENDERER_H
#define GLADOS_D3DX12RENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>  // to use ComPtr

#include "memory/Blob.h"
#include "platform/render/Renderer.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
    using Microsoft::WRL::ComPtr;
    class Logger;
    class GPUBuffer;
    class D3DX12Renderer : public Renderer, public Singleton<D3DX12Renderer> {
      public:
        D3DX12Renderer();
        ~D3DX12Renderer() override;

        bool initialize(int width, int height) override;
        void render(Renderable* _renderable) override;

        GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        ShaderProgram* createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource, const VertexBuffer* vertexBuffer) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, const VertexBuffer* vertexBuffer) override;
        Renderable* createRenderable(Mesh* mesh, Material* material) override;
        FrameBuffer* createFrameBuffer() override;
        RenderBuffer* createRenderBuffer() override;
        DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
        SamplerState* createSamplerState(const SamplerDescription& desc) override;
        RasterizerState* createRasterizerState(const RasterizerDescription& desc) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format) override;
        Texture2D* createTexture2D(PixelFormat format, Blob& data, const Color& colorKey) override;
        Texture2D* createTexture2D(PixelFormat format, Blob& data) override;
        Texture2D* createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) override;
        Texture3D* createTexture3D(const std::string& name) override;
        TextureCube* createTextureCube(const std::string& name, PixelFormat format) override;
        RenderTexture* createRenderTexture(const std::string& name) override;

        ComPtr<ID3D12Device> getDevice() const;
        ComPtr<ID3D12GraphicsCommandList> getCommandList() const;
        static std::string hresultToString(HRESULT hresult);
        static void getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool preferHighPerfAdapter = false);

      private:
        void PopulateCommandList();
        void WaitForPreviousFrame();

        static constexpr uint32_t renderTargetCount = 2;
        static Logger* logger;

        ComPtr<ID3D12Device> mDevice;
        ComPtr<ID3D12CommandQueue> mCommandQueue;
        ComPtr<IDXGISwapChain3> mSwapChain;
        ComPtr<ID3D12DescriptorHeap> mRenderTargetDescHeap;
        ComPtr<ID3D12DescriptorHeap> mDepthStencilDescHeap;
        ComPtr<ID3D12DescriptorHeap> mConstantBufferDescHeap;
        uint32_t mRenderTargetDescSize{0};
        uint32_t mDepthStencilDescSize{0};
        uint32_t mConstantBufferShaderResourceDescSize{0};
        ComPtr<ID3D12Resource> mRenderTargets[renderTargetCount];
        ComPtr<ID3D12Resource> mDepthStencil;
        ComPtr<ID3D12CommandAllocator> mCommandAllocator;
        ComPtr<ID3D12GraphicsCommandList> mCommandList;
        ComPtr<ID3D12PipelineState> mPipelineState;
        D3D12_VIEWPORT mScreenViewport{};
        D3D12_RECT mScissorRect{};

        // Synchronization objects.
        int mCurrBackBuffer{0};
        HANDLE mFenceEvent{nullptr};
        ComPtr<ID3D12Fence> mFence;
        uint64_t mFenceValue{1};
    };
}  // namespace GLaDOS

#endif

#endif