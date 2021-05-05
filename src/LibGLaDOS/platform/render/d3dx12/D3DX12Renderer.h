#ifndef GLADOS_D3DX12RENDERER_H
#define GLADOS_D3DX12RENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_WINDOW

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>  // to use ComPtr

#include "memory/StreamBuffer.h"
#include "platform/render/Renderer.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
  using Microsoft::WRL::ComPtr;
  class GPUBuffer;
  class D3DX12Renderer : public Renderer, public Singleton<D3DX12Renderer> {
  public:
    D3DX12Renderer() = default;
    ~D3DX12Renderer() override;

    bool initialize(int width, int height) override;
    void render(Renderable* _renderable) override;

    GPUBuffer* createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    GPUBuffer* createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) override;
    ShaderProgram* createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const VertexData* vertexData) override;
    Renderable* createRenderable(Mesh* mesh, Material* material) override;
    Mesh* createMesh(VertexData* vertexData, IndexData* indexData, PrimitiveType primitiveType, BufferUsage vertexUsage, BufferUsage indexUsage) override;
    Mesh* createMesh(const std::string& meshPath, PrimitiveType primitiveType, BufferUsage vertexUsage, BufferUsage indexUsage) override;
    FrameBuffer* createFrameBuffer() override;
    RenderBuffer* createRenderBuffer() override;
    DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
    SamplerState* createSamplerState(const SamplerDescription& desc) override;
    RasterizerState* createRasterizerState(const RasterizerDescription& desc) override;
    Texture2D* createTexture2D(const std::string& name, PixelFormat format, const Color& colorKey) override;
    Texture2D* createTexture2D(const std::string& name, PixelFormat format) override;
    Texture2D* createTexture2D(PixelFormat format, StreamBuffer& data, const Color& colorKey) override;
    Texture2D* createTexture2D(PixelFormat format, StreamBuffer& data) override;
    Texture2D* createTexture2D(uint32_t width, uint32_t height, PixelFormat format, unsigned char* data) override;
    Texture3D* createTexture3D(const std::string& name) override;
    TextureCube* createTextureCube(const std::string& name, PixelFormat format) override;
    RenderTexture* createRenderTexture(const std::string& name) override;

  private:
    static std::string hresultToString(HRESULT hr);
    static void getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool preferHighPerfAdapter = false);
    void PopulateCommandList();
    void WaitForPreviousFrame();
    static constexpr uint32_t frameCount = 2;

    ComPtr<ID3D12Device> mDevice;
    ComPtr<ID3D12CommandQueue> mCommandQueue;
    ComPtr<IDXGISwapChain3> mSwapChain;
    ComPtr<ID3D12DescriptorHeap> mRenderTargetDescHeap;
    uint32_t mRenderTargetDescSize{0};
    ComPtr<ID3D12Resource> mRenderTargets[frameCount];
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    ComPtr<ID3D12PipelineState> mPipelineState;

    // Synchronization objects.
    uint32_t mFrameIndex{0};
    HANDLE mFenceEvent{nullptr};
    ComPtr<ID3D12Fence> mFence;
    uint64_t mFenceValue{1};
  };
}  // namespace GLaDOS

#endif

#endif