#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cstring>
#include <optional>

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
    std::vector<const char*> vulkanExtensions;

    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    // logical device
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;


    // TODO move to renderer / window handler
    VkSurfaceKHR vulkanSurface;
    GLFWwindow* window = nullptr;
};
