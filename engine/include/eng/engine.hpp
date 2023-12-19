#pragma once

#include <iostream>

#define GLFW_INCLUDE_VULKAN // include vulkan with glfw
#include <GLFW/glfw3.h>

#include "api.hpp"

class EXPORTLIB Engine{
    public:

    // engine startup. Initializes GLFW and Vulkan
     int eng_startup();

    // engine startup. Stops GLFW and Vulkan
    int eng_shutdown();

    // engine loop
    int loop();

    private:

    VkResult createVulkanInstance();


    // public:

    private:
    VkInstance instance;
    // TODO move to renderer / window handler
    GLFWwindow* window = nullptr;
};
