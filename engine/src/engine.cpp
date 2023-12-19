#include "../include/eng/engine.hpp"


int Engine::eng_startup(){
    const int WIDTH = 800;
    const int HEIGHT = 800;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    createVulkanInstance();

    return 0;
}


int Engine::eng_shutdown(){
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();


    // TODO change return code
    return 0;
}

int Engine::loop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // TODO change return code
    return 0;
}

VkResult Engine::createVulkanInstance(){

    std::cout << "Creating Vulkan instance...\n";

    // if (enableValidationLayers && !checkValidationLayerSupport()) {
    //     throw std::runtime_error("validation layers requested, but not available!");
    // }

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

    createInfo.enabledLayerCount = 0;

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