#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <optional>

#define GLFW_INCLUDE_VULKAN // include vulkan with glfw
#include <GLFW/glfw3.h>

#include "api.hpp"
#include "logging.hpp"

struct QueueFamilyIndices {
    std::optional<u32> graphicsFamily;
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

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

    // public:

    private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    // TODO move to renderer / window handler
    GLFWwindow* window = nullptr;
};
