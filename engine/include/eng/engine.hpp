#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cstring>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN // include vulkan with glfw
#include <GLFW/glfw3.h>

#include "api.hpp"
#include "logging.hpp"

struct QueueFamilyIndices {
    std::optional<u32> graphicsFamily;
    std::optional<u32> presentFamily;

    bool isComplete(){
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                   VkDebugUtilsMessengerEXT debugMessenger, 
                                   const VkAllocationCallbacks* pAllocator);
 

class EXPORTLIB Engine{
    public:

    engResult startup();
    engResult shutdown();
    engResult loop();

    private:

    engResult createVulkanInstance();
    engResult enableValidationLayers();
    bool checkValidationLayerSupport(const std::vector<const char*>& awaitedLayers);
    std::vector<const char*> getRequiredExtensions();
    engResult setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    engResult pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice physicalDevice);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
    engResult createLogicalDevice();
    engResult createVulkanSurface();
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    engResult createSwapChain();
    engResult createImageViews();
    engResult createGraphicsPipeline();
    static std::vector<char> readShaderFile(const std::string& filename);
    std::optional<VkShaderModule> createShaderModule(std::vector<char>& shaderFileCodeBuffer);
    engResult createRenderPass();
    engResult createFramebuffers();
    engResult createCommandPool();
    engResult createCommandBuffer();
    engResult recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);
    void mainLoop();
    engResult drawFrame();
    engResult createSyncObjects();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

    // public:

    private:
    VkInstance instance;

    // TODO change if more layers will be used
    const std::vector<const char*> vulkanValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    std::vector<const char*> vulkanExtensions;

    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    // logical device
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;


    // TODO move to renderer / window handler
    VkSurfaceKHR vulkanSurface;
    GLFWwindow* window = nullptr;
};
