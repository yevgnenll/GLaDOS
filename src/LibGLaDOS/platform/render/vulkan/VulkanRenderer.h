#ifndef GLADOS_VULKANRENDERER_H
#define GLADOS_VULKANRENDERER_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_LINUX

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include "platform/render/Renderer.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
    struct VkSwapChainBuffer {
        VkImage image;
        VkImageView view;
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily = 0;
        uint32_t presentFamily = 0;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() {
            return graphicsFamilyHasValue && presentFamilyHasValue;
        }
    };

    class Logger;
    class GPUBuffer;
    class VulkanRenderer : public Renderer, public Singleton<VulkanRenderer> {
      public:
        VulkanRenderer();
        ~VulkanRenderer() override;

        bool initialize(int width, int height) override;
        void render(Renderable* _renderable, const Rect<real>& normalizedViewportRect) override;

        GPUBuffer* createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        GPUBuffer* createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) override;
        ShaderProgram* createShaderProgram(Shader* vertex, Shader* fragment, RenderPipelineState* renderPipelineState) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, RenderPipelineState* renderPipelineState) override;
        ShaderProgram* createShaderProgramFromFile(const std::string& vertexName, RenderPipelineState* renderPipelineState) override;
        Renderable* createRenderable(Mesh* mesh, Material* material) override;
        FrameBuffer* createFrameBuffer() override;
        DepthStencilState* createDepthStencilState(const DepthStencilDescription& desc) override;
        SamplerState* createSamplerState(const SamplerDescription& desc) override;
        RasterizerState* createRasterizerState(const RasterizerDescription& desc) override;
        RenderPipelineState* createRenderPipelineState(const RenderPipelineDescription& desc) override;
        RenderTexture* createRenderTexture(uint32_t width, uint32_t height, PixelFormat format) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, Blob& data) override;
        Texture2D* createTexture2D(const std::string& name, PixelFormat format, unsigned char* data) override;
        Texture3D* createTexture3D(const std::string& name) override;
        TextureCube* createTextureCube(const std::string& name, const Array<std::string, 6>& cubeNames, PixelFormat format) override;

        bool createInstance();
        std::vector<const char*> getRequiredInstanceExtensions();
        bool pickPhysicalDevice();
        bool createSurface();
        bool createLogicalDevice();
        bool createSwapChain(int width, int height);
        bool createCommandPool();

        bool checkValidationLayerSupport();
        bool isPhysicalDeviceSuitable(const VkPhysicalDevice& device);
        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);
        bool checkInstanceExtensionSupport(std::vector<const char*>& requiredExtensions);
        bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
        bool querySwapChainSupport(const VkPhysicalDevice& device, SwapChainSupportDetails& details);
        static VkExtent2D chooseSwapExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities);
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

      private:
        static Logger* logger;

        VkInstance mInstance{VK_NULL_HANDLE};
        VkSurfaceKHR mSurface{VK_NULL_HANDLE};
        VkDevice mLogicalDevice{VK_NULL_HANDLE};
        VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
        VkQueue mGraphicsQueue{VK_NULL_HANDLE};
        VkQueue mPresentQueue{VK_NULL_HANDLE};
        VkSwapchainKHR mSwapChain{VK_NULL_HANDLE};
        VkCommandPool mCommandPoo{VK_NULL_HANDLE};
        std::vector<VkSwapChainBuffer> mSwapChainBuffers;
        std::vector<const char*> mValidationLayers{"VK_LAYER_KHRONOS_validation"};
        std::vector<const char*> mDeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#ifdef DEBUG_BUILD
        bool mEnableValidationLayers{true};
#else
        bool mEnableValidationLayers{false};
#endif
    };
}  // namespace GLaDOS

#endif

#endif  //GLADOS_VULKANRENDERER_H
