#include <map>
#include <string>

#include "../include/eng/engine.hpp"


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                   VkDebugUtilsMessengerEXT debugMessenger, 
                                   const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// engine startup. Initializes GLFW and Vulkan
engResult Engine::startup(){
    const i32 WIDTH = 800;
    const i32 HEIGHT = 600;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    engResult res;

    res = createVulkanInstance();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = setupDebugMessenger();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = pickPhysicalDevice();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    return ENG_RESULT_SUCCESS;
}

// engine startup. Stops GLFW and Vulkan
engResult Engine::shutdown(){

    ENG_LOG_INFO("Engine shutting down...")
    //std::cout << "Shutting down...\n";

    if (ENG_VALIDATION_LAYERS_ENABLED) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return ENG_RESULT_SUCCESS;
}

// engine loop
engResult Engine::loop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }


    return ENG_RESULT_SUCCESS;
}

engResult Engine::createVulkanInstance(){

    ENG_LOG_INFO("Creating Vulkan instance...")

    // TODO change if more layers will be used
    const std::vector<const char*> awaitedValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    if (ENG_VALIDATION_LAYERS_ENABLED && !checkValidationLayerSupport(awaitedValidationLayers)) {
        ENG_LOG_ERROR("Validation layers requested, but not available!")
        return ENG_RESULT_FAILURE;
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
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (ENG_VALIDATION_LAYERS_ENABLED){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ENG_VALIDATION_LAYERS_ENABLED) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(awaitedValidationLayers.size());
        createInfo.ppEnabledLayerNames = awaitedValidationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
    if (res != VK_SUCCESS) {
        ENG_LOG_ERROR("Failed to create vulkan instance!")
        return ENG_RESULT_FAILURE;
    }
    else{
        ENG_LOG_INFO("Vulkan instance created")
        return ENG_RESULT_SUCCESS;
    }
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


std::vector<const char*> Engine::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // TODO check how does this work
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    //std::cout << extensions.size() << std::endl; 

    if (ENG_VALIDATION_LAYERS_ENABLED) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

engResult Engine::setupDebugMessenger(){
    if(!ENG_VALIDATION_LAYERS_ENABLED) return ENG_RESULT_FAILURE;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    VkResult res;
    res = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
    if (res != VK_SUCCESS){
        ENG_LOG_WARNING("failed to set up debug messenger")
        return ENG_RESULT_FAILURE;
    }
    else{
        return ENG_RESULT_SUCCESS;
    }

}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    ENG_SET_CONSOLE_TEXT_COLOUR_DARK_GREY
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT

    return VK_FALSE;
}

void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

engResult Engine::pickPhysicalDevice(){

    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0){
        ENG_LOG_ERROR("Physical device not found")
        return ENG_RESULT_FAILURE;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    // std::string str = std::to_string(deviceCount);
    // ENG_LOG_ERROR(str.c_str());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices){
        // choose the most sutable device
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }
    if (candidates.rbegin()->first > 0){
        physicalDevice = candidates.rbegin()->second;
    }
    else{
        ENG_LOG_ERROR("Failed to find a suitable GPU");
        return ENG_RESULT_FAILURE;
    }

    

    return ENG_RESULT_SUCCESS;
}

int Engine::rateDeviceSuitability(VkPhysicalDevice physicalDevice){

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    ENG_LOG_INFO_CYAN(deviceProperties.deviceName);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        ENG_LOG_INFO("This is discrete GPU");
    }
    else{
        ENG_LOG_INFO("This is not discrete GPU");
    }

    if (deviceFeatures.geometryShader){
        ENG_LOG_INFO("Geometry shaders are not supported");
    }
    else{
        ENG_LOG_INFO("Geometry shaders are supported");
    }

    int score = 0;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    // increase score if needed queue families are present
    if (indices.graphicsFamily.has_value()){
        score += 500;
    }



    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        score += 2000;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU){
        score += 1500;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU){
        score += 1000;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU){
        score += 500;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    std::string scoreStr = std::to_string(score);
    ENG_LOG_INFO("Device score:");
    ENG_LOG_INFO(scoreStr.c_str());

    return score;

}

QueueFamilyIndices Engine::findQueueFamilies(VkPhysicalDevice physicalDevice){
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int idx = 0;
    for (const auto& queueFamily: queueFamilies){
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = idx;
            break;
        }
        idx++;
    }

    return indices;
}