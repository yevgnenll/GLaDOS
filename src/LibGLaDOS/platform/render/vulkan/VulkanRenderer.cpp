#include "VulkanRenderer.h"

#ifdef PLATFORM_LINUX

#include "platform/linux/XWindowPlatform"

namespace GLaDOS {
    Logger* VulkanRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("VulkanRenderer");

    VulkanRenderer::VulkanRenderer() {
        setDestructionPhase(2);
    }

    VulkanRenderer::~VulkanRenderer() {
        if (mLogicalDevice != VK_NULL_HANDLE) {
            for (uint32_t i = 0; i < mSwapChainBuffers.size(); i++) {
                vkDestroyImageView(mLogicalDevice, mSwapChainBuffers[i].view, nullptr);
            }

            if (mSwapChain != VK_NULL_HANDLE) {
                vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);
            }

            if (mSurface != VK_NULL_HANDLE) {
                vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
            }

            if (mCommandPool != VK_NULL_HANDLE) {
                vkDestroyCommandPool(mLogicalDevice, mCommandPool, nullptr);
            }

            vkDeviceWaitIdle(mLogicalDevice);
            vkDestroyDevice(mLogicalDevice, nullptr);
        }

        vkDestroyInstance(mInstance, nullptr);
    }

    bool VulkanRenderer::initialize(int width, int height) {
        if (!createInstance()) {
            return false;
        }

        if (!createWindow()) {
            return false;
        }

        if (!createSurface()) {
            return false;
        }

        if (!pickPhysicalDevice()) {
            return false;
        }

        if (!createLogicalDevice()) {
            return false;
        }

        if (!createSwapChain(width, height)) {
            return false;
        }

        if (!createCommandPool()) {
            return false;
        }

        // create command buffer
        //    VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
        //    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
        //    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        //    commandBufferAllocateInfo.commandPool = mCommandPool;
        //    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        //    commandBufferAllocateInfo.commandBufferCount = 1;
        //    if (vkAllocateCommandBuffers(mLogicalDevice, &commandBufferAllocateInfo, &mCommandBuffer) != VK_SUCCESS) {
        //        std::cout << "Invalid command buffer" << std::endl;
        //        return false;
        //    }
        //    if (mCommandBuffer != VK_NULL_HANDLE) {
        //        vkFreeCommandBuffers(mLogicalDevice, mCommandPool, 1, &mCommandBuffer);
        //    }
    }

    void VulkanRenderer::render(Renderable* _renderable, const Rect<real>& normalizedViewportRect) {

    }

    GPUBuffer* VulkanRenderer::createGPUVertexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        return nullptr;
    }

    GPUBuffer* VulkanRenderer::createGPUIndexBuffer(GPUBufferUsage usage, void* data, std::size_t size) {
        return nullptr;
    }

    ShaderProgram* VulkanRenderer::createShaderProgram(Shader* vertex, Shader* fragment, RenderPipelineState* renderPipelineState) {
        return nullptr;
    }

    ShaderProgram* VulkanRenderer::createShaderProgramFromFile(const std::string& vertexName, const std::string& fragmentName, RenderPipelineState* renderPipelineState) {
        return nullptr;
    }

    ShaderProgram* VulkanRenderer::createShaderProgramFromFile(const std::string& vertexName, RenderPipelineState* renderPipelineState) {
        return nullptr;
    }

    Renderable* VulkanRenderer::createRenderable(Mesh* mesh, Material* material) {
        return nullptr;
    }

    FrameBuffer* VulkanRenderer::createFrameBuffer() {
        return nullptr;
    }

    DepthStencilState* VulkanRenderer::createDepthStencilState(const DepthStencilDescription& desc) {
        return nullptr;
    }

    SamplerState* VulkanRenderer::createSamplerState(const SamplerDescription& desc) {
        return nullptr;
    }

    RasterizerState* VulkanRenderer::createRasterizerState(const RasterizerDescription& desc) {
        return nullptr;
    }

    RenderPipelineState* VulkanRenderer::createRenderPipelineState(const RenderPipelineDescription& desc) {
        return nullptr;
    }

    RenderTexture* VulkanRenderer::createRenderTexture(uint32_t width, uint32_t height, PixelFormat format) {
        return nullptr;
    }

    Texture2D* VulkanRenderer::createTexture2D(const std::string& name, PixelFormat format) {
        return nullptr;
    }

    Texture2D* VulkanRenderer::createTexture2D(const std::string& name, PixelFormat format, Blob& data) {
        return nullptr;
    }

    Texture2D* VulkanRenderer::createTexture2D(const std::string& name, PixelFormat format, unsigned char* data) {
        return nullptr;
    }

    Texture3D* VulkanRenderer::createTexture3D(const std::string& name) {
        return nullptr;
    }

    TextureCube* VulkanRenderer::createTextureCube(const std::string& name, const Array<std::string, 6>& cubeNames, PixelFormat format) {
        return nullptr;
    }

    bool XWindowPlatform::createInstance() {
        if (mEnableValidationLayers && !checkValidationLayerSupport()) {
            return false;
        }

        std::vector<const char*> extensions = getRequiredInstanceExtensions();
        if (!checkInstanceExtensionSupport(extensions)) {
            return false;
        }

        VkApplicationInfo applicationInfo{};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_1; // using at least Vulkan version 1.1
        applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        applicationInfo.pApplicationName = nullptr;
        applicationInfo.pEngineName = "GLaDOS";

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        if (mEnableValidationLayers) {
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
        } else {
            instanceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
            std::cout << "Failed to create vulkan instance" << std::endl;
            return false;
        }

        return true;
    }

    std::vector<const char*> XWindowPlatform::getRequiredInstanceExtensions() {
        std::vector<const char*> requiredExtensions{VK_KHR_XLIB_SURFACE_EXTENSION_NAME};
        if (mEnableValidationLayers) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return requiredExtensions;
    }

    bool XWindowPlatform::pickPhysicalDevice() {
        uint32_t physicalDeviceCount = 0;
        if (vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr) != VK_SUCCESS || physicalDeviceCount < 1) {
            std::cout << "Failed to find Physical Device with Vulkan support!" << std::endl;
            return false;
        }

        std::cout << "Physical Device count: " << physicalDeviceCount << std::endl;
        std::vector<VkPhysicalDevice> physicalDevices{physicalDeviceCount};
        if (vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
            std::cout << "Failed to find Physical Device with Vulkan support!" << std::endl;
            return false;
        }

        for (const VkPhysicalDevice& device : physicalDevices) {
            if (isPhysicalDeviceSuitable(device)) {
                mPhysicalDevice = device;
                break;
            }
        }

        if (mPhysicalDevice == VK_NULL_HANDLE) {
            std::cout << "Failed to find Physical Device with Vulkan support!" << std::endl;
            return false;
        }

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(mPhysicalDevice, &deviceProperties);
        std::cout << "Physical Device: " << deviceProperties.deviceName << std::endl;

        return true;
    }

    bool XWindowPlatform::createSurface() {
        VkXlibSurfaceCreateInfoKHR surfaceCreateInfo{};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.dpy = XWindowPlatform::getXWindowPlatform()->getDisplay();
        surfaceCreateInfo.window = XWindowPlatform::getXWindowPlatform()->getWindow();
        if (vkCreateXlibSurfaceKHR(mInstance, &surfaceCreateInfo, nullptr, &mSurface) != VK_SUCCESS) {
            std::cout << "Failed to create X11 surface" << std::endl;
            return false;
        }

        return true;
    }

    bool XWindowPlatform::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(mPhysicalDevice);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies { indices.graphicsFamily, indices.presentFamily };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();
        if (mEnableValidationLayers) {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
        } else {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice) != VK_SUCCESS) {
            std::cout << "Failed to create logical device" << std::endl;
            return false;
        }

        vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily, 0, &mGraphicsQueue);
        vkGetDeviceQueue(mLogicalDevice, indices.presentFamily, 0, &mPresentQueue);

        return true;
    }

    bool XWindowPlatform::createSwapChain(int width, int height) {
        SwapChainSupportDetails details;
        if (!querySwapChainSupport(mPhysicalDevice, details)) {
            return false;
        }
        VkExtent2D swapChainExtent = XWindowPlatform::chooseSwapExtent(width, height, details.capabilities);
        VkSurfaceFormatKHR surfaceFormat = XWindowPlatform::chooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = XWindowPlatform::chooseSwapPresentMode(details.presentModes);

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        // vkCreateXlibSurfaceKHR 함수로 만들어진 surface
        swapChainCreateInfo.surface = mSurface;
        // 스왑 체인의 이미지 수 (이중 혹은 삼중 버퍼링을 위해선 2와 3으로 세팅)
        swapChainCreateInfo.minImageCount = details.capabilities.minImageCount;
        // 표현 가능한 이미지의 형식과 컬러 공간
        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace; // VK_COLORSPACE_SRGB_NONLINEAR_KHR (sRGB 타입 이미지 라면 sRGB 비선형 자료를 기대한다는 뜻)
        // 스왑 체인의 크기(버퍼의 크기)
        swapChainCreateInfo.imageExtent.width = swapChainExtent.width;
        swapChainCreateInfo.imageExtent.height = swapChainExtent.height;
        // 이미지가 프리젠테이션되기 전에 어떻게 변환되는지를 설명
        swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        // 알파 조합이 어떻게 되는지 처리
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        // 각 이미지의 레이어의 수를 설정
        swapChainCreateInfo.imageArrayLayers = 1;
        // 프리젠테이션을 할 때, 윈도우와 스왑체인간의 동기화가 어떻게 될 지 설정
        swapChainCreateInfo.presentMode = presentMode;
        // surface와 결합되어 존재하는 스왑 체인을 vulkan에 재활용을 위해서 전달하는 데 사용
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
        // surface가 보이지 않을 때 최적화를 위해서 사용. VK_TRUE라면, 이미지의 해당 부분을 렌더링 연산에서 제거
        swapChainCreateInfo.clipped = VK_TRUE;
        // 이미지가 어떻게 사용될지(프리젠테이션 원본으로 사용하는 것에 추가하여)를 설정
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
        // 이미지가 어떻게 큐 사이에서 공유되는지를 설명
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // VK_SHARING_MODE_CONCURRENT
        // sharingMode가 VK_SHARING_MODE_EXCLUSIVE 일 때 무시된다
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;

        if (vkCreateSwapchainKHR(mLogicalDevice, &swapChainCreateInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
            std::cout << "Invalid swapChain" << std::endl;
            return false;
        }

        uint32_t swapChainImageCount = 0;
        if (vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &swapChainImageCount, nullptr) != VK_SUCCESS || swapChainImageCount < 1) {
            std::cout << "Invalid swapChain count" << std::endl;
            return false;
        }

        mSwapChainBuffers.resize(swapChainImageCount);
        std::vector<VkImage> swapChainImages{swapChainImageCount};
        if (vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &swapChainImageCount, swapChainImages.data()) != VK_SUCCESS) {
            std::cout << "Invalid swapChain image" << std::endl;
            return false;
        }

        for (uint32_t i = 0; i < swapChainImageCount; i++) {
            mSwapChainBuffers[i].image = swapChainImages[i];
        }

        for (uint32_t i = 0; i < swapChainImageCount; i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = mSwapChainBuffers[i].image;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = surfaceFormat.format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(mLogicalDevice, &imageViewCreateInfo, nullptr, &mSwapChainBuffers[i].view) != VK_SUCCESS) {
                std::cout << "Failed to create swapChain image view" << std::endl;
                return false;
            }
        }

        return true;
    }

    bool XWindowPlatform::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(mPhysicalDevice);

        VkCommandPoolCreateInfo commandPoolCreateInfo{};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(mLogicalDevice, &commandPoolCreateInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
            std::cout << "Failed to create command pool" << std::endl;
            return false;
        }

        return true;
    }

    void XWindowPlatform::mainLoop() {
        XEvent event;
        while (1) {
            XNextEvent(mDisplay, &event);

            if (event.type == Expose) {

            }

            if (event.type == KeyPress) {
                std::cout << "hello world" << std::endl;
            }
        }
    }

    bool XWindowPlatform::checkValidationLayerSupport() {
        uint32_t instanceLayerCount = 0;
        if (vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr) != VK_SUCCESS || instanceLayerCount < 1) {
            std::cout << "Invalid instance layers" << std::endl;
            return false;
        }

        std::vector<VkLayerProperties> availableLayers{instanceLayerCount};
        if (vkEnumerateInstanceLayerProperties(&instanceLayerCount, availableLayers.data()) != VK_SUCCESS) {
            std::cout << "Invalid instance layers" << std::endl;
            return false;
        }

        for (const char* layerName : mValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                std::cout << "Missing validation layer: " << layerName << std::endl;
                return false;
            }
        }

        return true;
    }

    bool XWindowPlatform::isPhysicalDeviceSuitable(const VkPhysicalDevice& device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport;
            if (!querySwapChainSupport(device, swapChainSupport)) {
                return false;
            }
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    QueueFamilyIndices XWindowPlatform::findQueueFamilies(const VkPhysicalDevice& device) {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                indices.graphicsFamilyHasValue = true;
            }

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
                indices.presentFamilyHasValue = true;
            }

            if (indices.isComplete()) {
                break;
            }
            ++i;
        }

        return indices;
    }

    bool XWindowPlatform::checkInstanceExtensionSupport(std::vector<const char*>& requiredExtensions) {
        uint32_t instanceExtensionsCount = 0;
        if (vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr) != VK_SUCCESS || instanceExtensionsCount < 1) {
            std::cout << "Invalid instance extensions" << std::endl;
            return false;
        }

        std::vector<VkExtensionProperties> availableExtensions{instanceExtensionsCount};
        if (vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, availableExtensions.data()) != VK_SUCCESS) {
            std::cout << "Invalid instance extensions" << std::endl;
            return false;
        }

        std::set<std::string> extensions(requiredExtensions.begin(), requiredExtensions.end());
        for (const auto& extension : availableExtensions) {
            extensions.erase(extension.extensionName);
        }

        for (const std::string& missingExtension : extensions) {
            std::cout << "Missing instance Extension: " << missingExtension << std::endl;
        }

        return extensions.empty();
    }

    bool XWindowPlatform::checkDeviceExtensionSupport(const VkPhysicalDevice& device) {
        uint32_t extensionCount;
        if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
            std::cout << "Failed to check device extension" << std::endl;
            return false;
        }

        std::vector<VkExtensionProperties> availableExtensions{extensionCount};
        if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()) != VK_SUCCESS) {
            std::cout << "Failed to check device extension" << std::endl;
            return false;
        }

        std::set<std::string> requiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool XWindowPlatform::querySwapChainSupport(const VkPhysicalDevice& device, SwapChainSupportDetails& details) {
        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities) != VK_SUCCESS) {
            return false;
        }

        uint32_t surfaceFormatCount = 0;
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &surfaceFormatCount, nullptr) != VK_SUCCESS) {
            return false;
        }

        if (surfaceFormatCount != 0) {
            details.formats.resize(surfaceFormatCount);
            if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &surfaceFormatCount, details.formats.data()) != VK_SUCCESS) {
                return false;
            }
        }

        uint32_t presentModeCount = 0;
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr) != VK_SUCCESS) {
            return false;
        }

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS) {
                return false;
            }
        }

        return true;
    }

    VkExtent2D XWindowPlatform::chooseSwapExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent = {width, height};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }

    VkSurfaceFormatKHR XWindowPlatform::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR XWindowPlatform::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
}

#endif