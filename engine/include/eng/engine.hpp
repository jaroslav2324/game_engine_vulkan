#pragma once

#include <iostream>
#include <vector>
#include <cstring>

#define GLFW_INCLUDE_VULKAN // include vulkan with glfw
#include <GLFW/glfw3.h>

#include "api.hpp"

class EXPORTLIB Engine{
    public:

    int startup();
    int shutdown();
    int loop();

    private:

    VkResult createVulkanInstance();
    VkResult enableValidationLayers();
    bool checkValidationLayerSupport(const std::vector<const char*>& awaitedLayers);


    // public:

    private:
    VkInstance instance;
    // TODO move to renderer / window handler
    GLFWwindow* window = nullptr;
};
