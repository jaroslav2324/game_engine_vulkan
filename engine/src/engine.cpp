#include "../include/eng/engine.hpp"

// engine startup. Initializes GLFW and Vulkan
int Engine::startup(){
    const int WIDTH = 800;
    const int HEIGHT = 600;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    createVulkanInstance();

    return 0;
}

// engine startup. Stops GLFW and Vulkan
int Engine::shutdown(){
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();


    // TODO change return code
    return 0;
}

// engine loop
int Engine::loop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // TODO change return code
    return 0;
}

VkResult Engine::createVulkanInstance(){

    std::cout << "Creating Vulkan instance...\n";

    // TODO change if more layers will be used
    const std::vector<const char*> awaitedValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    if (ENG_VALIDATION_LAYERS_ENABLED && !checkValidationLayerSupport(awaitedValidationLayers)) {
        std::cout << "validation layers requested, but not available!\n";
        // TODO change to known
        return VK_ERROR_UNKNOWN;
    }

    VkApplicationInfo info{};
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Hello triangle";
    info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    info.pEngineName = "Just engine";
    info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &info;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (ENG_VALIDATION_LAYERS_ENABLED) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(awaitedValidationLayers.size());
        createInfo.ppEnabledLayerNames = awaitedValidationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // TODO change return code
    VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
    if (res != VK_SUCCESS) {
        std::cout << "failed to create vulkan instance!\n";
        return res;
    }
    std::cout << "Created\n";

    // TODO change return code
    return VK_SUCCESS;
}

// compares awaited layers with available layers. returns true if all awaited layers found
bool Engine::checkValidationLayerSupport(const std::vector<const char*>& awaitedLayers) {

    VkResult result;
    uint32_t layerCount;
    result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    if (result != VK_SUCCESS) return false;

    std::vector<VkLayerProperties> availableLayers(layerCount);
    result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    if (result != VK_SUCCESS) return false;

    for (const char* layerName : awaitedLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;    
}
